#include <string>
#include <cstdio>
#include "../include/ini.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <utility>
#include <sys/stat.h>
#include <bits/stdc++.h>
#include <unordered_map>

using namespace std;
#define READ_ERROR 1;
#define MISSING_KEY 3;
#define FILE_NOT_LOADED 4;
#define GENERIC_ERROR 255;
#define MAX_MMAP_LENGTH 1024 * 1024 * 250

string file;
const char *filename;
int fd;
off_t filesize;
std::unordered_map<string, string> map_data;
char *mmap_obj;

std::string trim(const std::string &line)

{
    const char *WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : line.substr(start, end - start + 1);
}

// load and parse in volatile memory a INI resource file located in Linux filesystem
unsigned short load_resurce(const std::string &path)
{
    FILE *fp;
    filename = path.c_str();
    const size_t file_size = 4096; // file page size in kb.
    file = filename;
    struct stat buffer; // buf argument is a pointer to a stat structure

    int file_status = stat(filename, &buffer);

    if (file_status == 0)
    {
        fd = open(filename, O_RDONLY);
    }
    else if (file_status == -1)
    {
        perror("Error opening file");
        close(fd);
        return EXIT_FAILURE;
    }
    if (fd < 0)
    {
        perror("file descriptor error.");
        close(fd);
        return READ_ERROR;
    };

    //  Get the size of the file
    off_t filesize = lseek(fd, 0, SEEK_END); // SEEK_END The file offset is set to the size of the file plus offset  bytes.

    // Map the file into memory
    char *mmap_obj = (char *)(mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0));

    if (mmap_obj == MAP_FAILED)
    {
        perror("Error mapping file");
        close(fd);
        return READ_ERROR; // read error
    }

    if (mmap_obj == nullptr)
    {
        close(fd);
        perror("Error mmapping the file");
        return FILE_NOT_LOADED; // resource file has not been loaded
    }
    else
    {
        stringstream ss(mmap_obj);
        string s, str;
        std::string line;

        while (std::getline(ss, line, '\n'))
        {
            //  Trim whitespace
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            // Skip comments and empty lines
            if (line.empty() || line[0] == ';')
                continue;

            // Section header
            if (line[0] == '[')
            {
                map_data[line] = "section";
            }

            // Key-value pairs
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos)
            {
                std::string key = line.substr(0, delimiterPos);
                key = trim(key);
                std::string value = line.substr(delimiterPos + 1);
                value = trim(value);
                map_data[key] = value;
            }
        }

        if (munmap(mmap_obj, filesize) == -1)
        {
            close(fd);
            perror("Error un-mmapping the file");
            return GENERIC_ERROR;
        }

        if (fd)
        {
            close(fd);
        }
    }
    return EXIT_SUCCESS;
}

// allows an application to retrive the value of a key available in apreviously loaded INI file
unsigned short get_value(const std::string &key, std::string &value)
{
    if (map_data.empty())
    {
        return FILE_NOT_LOADED; // resource file has not been loaded
    }

    if (map_data.count(key) == 0)
    {
        return MISSING_KEY; // missing key
    }
    else
    {
        value = map_data.at(key);
        return EXIT_SUCCESS;
    }
}

// Store the value of a key in a previously loaded INI file.
// This adds or replace the new key/value pair both in the volatile memory and in the ini file on the filesystem.
unsigned short set_value(const std::string &key, std::string &value)
{

    int result;
    size_t textsize;
    char *filen = file.data();
    struct stat buffer;
    int file_status = stat(filen, &buffer);

    if (file_status == 0)
    {
        fd = open(filen, O_RDWR, 0644);
    }
    else if (file_status == -1)
    {
        close(fd);
        return EXIT_FAILURE;
    }
    if (fd < 0)
    {
        perror("file descriptor error.");
        close(fd);
        return READ_ERROR;
    };

    off_t filesize = lseek(fd, 0, SEEK_END);
    mmap_obj = static_cast<char *>(mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0));

    if (mmap_obj == MAP_FAILED)
    {
        close(fd);
        return GENERIC_ERROR;
    }

    if (mmap_obj == nullptr)
    {
        close(fd);
        return FILE_NOT_LOADED; // resource file has not been loaded
    }
    else
    {
        string str;
        string str_mmap = mmap_obj;

        if (map_data.find(key) != map_data.end())
        {
            string next_lines, previous_lines, current_line;
            size_t key_pos = str_mmap.find(key);
            previous_lines = str_mmap.substr(0, key_pos);
            string current_value = map_data.at(key);
            map_data.at(key) = value;
            size_t value_pos = str_mmap.find(current_value);
            string space = "\n";
            int start = str_mmap.size() - value_pos + 1;
            next_lines = str_mmap.substr((value_pos + current_value.size()));
            current_line = key + " = " + value;
            str = previous_lines + current_line + next_lines;
        }
        else
        {

            map_data[key] = value;
            str = str_mmap + "\n" + key + " = " + value + "\n";
        }

        if (munmap(mmap_obj, filesize) == -1)
        {
            close(fd);
            return GENERIC_ERROR;
        }

        const char *text = str.c_str();

        //  calc text size
        textsize = strlen(text + 1);
        result = lseek(fd, textsize, SEEK_SET);

        if (result == -1)
        {
            close(fd);
            perror("Error calling lseek() to 'stretch' the file");
            return GENERIC_ERROR;
        }
        result = write(fd, "", 1);
        if (result != 1)
        {
            close(fd);
            perror("Error writing last byte of the file");
            return GENERIC_ERROR;
        }
        mmap_obj = static_cast<char *>(mmap(NULL, textsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

        if (filesize > str.size())
        {
            int diff = filesize - textsize;
            for (int i = 0; i < diff; ++i)
            {
                str.push_back(' ');
            }
        }
        for (int i = 0; i < str.size(); ++i)
        {
            mmap_obj[i] = str[i];
        }
    }

    if (munmap(mmap_obj, textsize) == -1)
    {
        close(fd);
        perror("Error un-mmapping the file");
        return GENERIC_ERROR;
    }

    close(fd);
    return 0;
}