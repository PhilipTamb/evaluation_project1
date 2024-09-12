#include <string>
#include <cstdio> // getc
#include "../include/ini.hpp"
#include <fcntl.h>
#include <sys/mman.h> //open, posix_fallocate
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <mutex>
#include <utility>
#include <sys/stat.h>

#include <bits/stdc++.h>

using namespace std;
#define READ_ERROR 1;
#define MISSING_KEY 3;
#define FILE_NOT_LOADED 4;
#define GENERIC_ERROR 255;
#define MAX_MMAP_LENGTH 1024 * 1024 * 250

string file;
std::string trim(const std::string &line)
{
    const char *WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : line.substr(start, end - start + 1);
}

// load and parse in volatile memory a INI resource file located in Linux filesystem
unsigned short Ini::load_resurce(const std::string &path)
{

    FILE *fp;
    filename = path.c_str();
    const size_t file_size = 4096; // file page size in kb.
                                   // FILE *fp = fopen(filename, "r");
    file = filename;
    struct stat buffer; // buf argument is a pointer to a stat structure
    // obtain file status information for a file name. success return 0, otherwise, -1 and errno set
    int file_status = stat(filename, &buffer);

    cout << "file_status:  " << file_status << endl;
    cout << "path:  " << path << endl;

    if (file_status == 0)
    {
        fd = open(filename, O_RDONLY);
    }
    else if (file_status == -1)
    {
        // perror("Error opening file");
        close(fd);
        return EXIT_FAILURE;
    }
    if (fd < 0)
    {
        // perror("file descriptor error.");
        close(fd);
        return READ_ERROR;
    };

    // cout << "fd:  " << fd << endl;
    //  For mmap we need to stretch file
    //  Get the size of the file
    off_t filesize = lseek(fd, 0, SEEK_END); // SEEK_END The file offset is set to the size of the file plus offset  bytes.

    // Map the file into memory
    // (starting address(If addr is NULL, then the kernel chooses the (page-aligned) address at which to create the mapping), length of the mapping, )
    // char *mmap_obj = static_cast<char *>(mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0));
    char *mmap_obj = (char *)(mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0));
    // mmap_obj = static_cast<char *>(mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (mmap_obj == MAP_FAILED)
    {
        // perror("Error mapping file");
        close(fd);
        return READ_ERROR; // read error
    }

    // Access the file content through the mmapped memory
    // cout << "File content: " << mmap_obj << endl;

    // int end_file = filesize * sizeof(char);

    if (mmap_obj == nullptr)
    {
        close(fd);
        // perror("Error mmapping the file");
        return FILE_NOT_LOADED; // resource file has not been loaded
    }
    else
    {
        stringstream ss(mmap_obj);
        string s, str;
        std::string line, currentSection;
        while (std::getline(ss, line, '\n'))
        {
            // cout << " LINE: " << line << endl;
            //  Trim whitespace
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            // line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

            // Skip comments and empty lines
            if (line.empty() || line[0] == ';')
                continue;

            // Section header
            if (line[0] == '[')
            {
                // currentSection = line; // line.substr(1, line.find(']') - 1);
                // cout << " CURRENT SECTION: " << currentSection << endl;
                data[line] = "section";
                // continue;
            }

            // Key-value pairs
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos)
            {
                std::string key = line.substr(0, delimiterPos);
                // int pos;
                // pos = key.find_first_not_of(" ");
                // if (pos)
                // {
                //     key = key[pos];
                // }
                // pos = key.find_last_not_of(" ");
                // if (pos > 0)
                // {

                //     key = key.substr(0, pos);
                // }
                // if (key[0] == ' ')
                //     key.erase(key[0]);
                // if (key[key.size() - 1] == ' ')
                //     key.erase(key[key.size() - 1]);
                key = trim(key);
                std::string value = line.substr(delimiterPos + 1);
                value = trim(value);
                // pos = value.find_first_not_of(" ");
                // if (pos)
                // {
                //     value = value[pos];
                // }
                // pos = value.find_last_not_of(" ");
                // if (pos > 0)
                // {

                //     value = value.substr(0, pos);
                // }
                data[key] = value;
                cout << " key: " << key << "              value: " << value << endl;
            }
        }

        if (munmap(mmap_obj, filesize) == -1) // if (munmap(mmap_obj, textsize) == -1)
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

    // for (auto i : this->data)
    //     cout << "'" << i.first << "' \t\t\t'" << i.second << "'" << endl;

    // string word;
    //  while (ss >> word)
    //  { // Extract word from the stream.
    //      cout << " word : " << word << endl;
    //  }

    // if (munmap(mmap_obj, filesize) == -1)
    // {
    //     close(fd);
    //     return GENERIC_ERROR;
    // }
    // close(fd);
    cout << "file name:" << filename << endl;
    return EXIT_SUCCESS;
}

// Ini::~Ini()
// {

//     if (this->mmap_obj)
//     {
//         munmap(this->mmap_obj, this->filesize);
//     }
//     if (this->fd)
//     {
//         close(this->fd);
//     }
// };

// allows an application to retrive the value of a key available in apreviously loaded INI file
unsigned short Ini::get_value(const std::string &key, std::string &value)
{
    cout << " set_value      key  :    " << key << endl;

    if (data.empty())
    {
        cout << "set_value   mmap_obj == nullptr" << endl;
        // close(fd);
        //  perror("Error mmapping the file");
        return FILE_NOT_LOADED; // resource file has not been loaded
    }

    if (data.count(key) == 0)
    {
        cout << "data.count(key) == 0" << endl;
        // close(fd);
        return MISSING_KEY; // missing key
    }
    else
    {
        cout << "set_value " << endl;
        value = data.at(key);
        cout << "value: " << value << endl;
        // string v = data.at(key);
        // cout << "value: " << v << endl;
        // const char *ch = v.c_str();
        // char *b;
        // memcpy(b, ch, strlen(ch));
        // value = string(b);
        // close(fd);
        cout << "file name:" << filename << endl;
        return EXIT_SUCCESS;
    }
}

// Store the value of a key in a previously loaded INI file.
// This adds or replace the new key/value pair both in the volatile memory and in the ini file on the filesystem.
unsigned short Ini::set_value(const std::string &key, std::string &value)
{
    cout << "set_value   key:" << key << endl;
    cout << "set_value   value:" << value << endl;
    int result;
    size_t textsize;
    // off_t offset, pa_offset;
    // std::string key_str;

    cout << "file name:" << this->filename << endl;
    cout << "file name:" << file << endl;
    char *filen = file.data();
    cout << "filen:" << filen << endl;

    struct stat buffer; // buf argument is a pointer to a stat structure
    // obtain file status information for a file name. success return 0, otherwise, -1 and errno set
    int file_status = stat(filen, &buffer);
    cout << "file_status:" << file_status << endl;
    if (file_status == 0)
    {
        fd = open(filen, O_RDWR, 0644);
    }
    else if (file_status == -1) // entra qui dentro
    {
        // perror("Error opening file");
        cout << "exit failue" << endl;
        close(fd);
        return EXIT_FAILURE;
    }
    if (fd < 0)
    {
        cout << "fd < 0" << endl;
        perror("file descriptor error.");
        close(fd);
        return READ_ERROR;
    };

    cout << "fd:  " << fd << endl;
    // For mmap we need to stretch file
    // Get the size of the file
    // off_t filesize = lseek(fd, 0, SEEK_END); // SEEK_END The file offset is set to the size of the file plus offset  bytes.
    off_t filesize = lseek(fd, 0, SEEK_END);
    // pa_offset = filesize;

    // we need to share the mapping (to be able to write inside the file thru the virtual address space)
    // mmap_obj = static_cast<char *>(mmap(NULL, MAX_MMAP_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    cout << "before mmap" << endl;
    mmap_obj = static_cast<char *>(mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0));

    if (mmap_obj == MAP_FAILED)
    {
        close(fd);

        perror("Error mmapping the file");
        return GENERIC_ERROR;
    }

    cout << "- set_value funct " << endl;
    if (mmap_obj == nullptr)
    {
        // cout << "- if " << endl;
        close(fd);
        // perror("Error mmapping the file");
        return FILE_NOT_LOADED; // resource file has not been loaded
    }
    // else if (data.count(key) == 0)
    // {
    //     cout << "- else if " << endl;
    //     return MISSING_KEY; // missing key
    // }
    else
    {
        string str;
        string str_mmap = mmap_obj;
        cout << "- key : " << key << endl;
        if (data.find(key) != data.end())
        {
            cout << "data.find(key) != data.end()" << endl;
            // stringstream ss(mmap_obj);

            string next_lines, previous_lines, current_line;
            std::string line, currentSection;

            size_t key_pos = str_mmap.find(key);
            previous_lines = str_mmap.substr(0, key_pos);
            string current_value = data.at(key);
            data.at(key) = value;

            size_t value_pos = str_mmap.find(current_value);
            // cout << "- current_value : " << current_value << endl;
            //  size_t value_pos = str_mmap.find_last_not_of(current_value);
            //  cout << "- key_pos : " << key_pos << endl;
            //  cout << "- value_pos : " << value_pos << endl;
            //  cout << "- str_mmap : " << str_mmap << endl;
            string space = "\n";
            int start = str_mmap.size() - value_pos + 1;
            // cout << "- start : " << start << endl;
            next_lines = str_mmap.substr((value_pos + current_value.size())); // next_lines = str_mmap.substr(value_pos + value.size(), str_mmap.size() - 1);
            current_line = key + " = " + value;
            cout << endl;
            // cout << "- previous_lines : " << previous_lines << endl;
            // cout << " - next_lines: " << next_lines << endl;
            // cout << "- current_line : " << current_line << endl;
            str = previous_lines + current_line + next_lines;
        }
        else
        //(data.find(key) == data.end())
        {
            cout << "data.find(key) == data.end()" << endl;
            data[key] = value;
            str = str_mmap + "\n" + key + " = " + value + "\n";
        }

        if (munmap(mmap_obj, filesize) == -1)
        {
            close(fd);
            return GENERIC_ERROR;
        }

        // cout << "str : " << str << endl;
        const char *text = str.c_str();
        // cout << "text : " << text << endl;
        // cout << endl;
        //  calc text size
        textsize = strlen(text + 1);
        // offset = strlen(text) + 1;
        // pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1); /* offset for mmap() must be page aligned */

        // file size of the content that will be write, move to start position byte
        // result = lseek(fd, textsize, SEEK_SET);
        result = lseek(fd, textsize, SEEK_SET);

        // if (textsize < pa_offset)
        // {
        //     int diff = pa_offset - textsize;
        //     offset = textsize - diff - 10;
        // }

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

        // mmap_obj = (char *)mmap(NULL, textsize + offset - pa_offset, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mmap_obj = (char *)mmap(NULL, textsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        mmap_obj = static_cast<char *>(mmap(NULL, textsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

        cout << "filesize: " << filesize << endl;
        cout << "textsize: " << textsize << endl;

        if (filesize > str.size())
        {
            int diff = filesize - textsize;
            cout << "diff: " << diff << endl;
            for (int i = 0; i < diff; ++i)
            {
                str.push_back(' ');
            }
            cout << "str.size(): " << str.size() << endl;
        }
        for (int i = 0; i < str.size(); ++i)
        {
            mmap_obj[i] = str[i];
        }
        // memcpy(mmap_obj, text, strlen(text));
        // msync(mmap_obj, offset, MS_SYNC); // msync(mmap_obj, textsize, MS_SYNC);
    }

    // cout << "File content set_value function: " << mmap_obj << endl;
    // cout << endl;

    if (munmap(mmap_obj, textsize) == -1) // if (munmap(mmap_obj, textsize) == -1)
    {
        close(fd);
        perror("Error un-mmapping the file");
        return GENERIC_ERROR;
    }

    close(fd);
    return 0;
}