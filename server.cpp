
// #include <stdio.h>

#include <stdlib.h>
#include <string>
#include <iostream>
#include "include/ini.hpp"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include <arpa/inet.h> // inet_addr
//  #include <stdint.h>
//  #include <string.h>

using namespace std;
string path;
string default_path = "/home/userver/evaluation_project1/example/example.ini";

bool isASCII(const std::string &s)
{
    return !std::any_of(s.begin(), s.end(), [](char c)
                        { return static_cast<unsigned char>(c) > 127; });
}

bool invalidChar(char c)
{
    return !(c >= 0 && c < 128);
}

int main(int argc, char *argv[])
{

    // if (argc > 1)
    // {
    //     path = argv[1];
    //     cout << "path: " << path << " " << endl;
    // }
    // else
    // {
    //     path = default_path;
    // }

    Ini *ini = new Ini();
    unsigned short res;

    // path = "/home/userver/evaluation_project1/example/example.ini";
    // res = ini->Ini::load_resurce(path);
    // cout << "load_resurce return: " << res << endl;

    // string buf;
    // res = ini->Ini::get_value("section.foo.bar", buf);
    // cout << "get_value return: " << buf << endl;

    // string str1 = "section.color.red";
    // string str2 = "roses are red and blue";
    // res = ini->Ini::set_value(str1, str2);
    // cout << "set_value return: " << res << endl;

    // str1 = "section.foo.bar";
    // str2 = "2048";
    // res = ini->Ini::set_value(str1, str2);
    // cout << "set_value return: " << res << endl;

    // str1 = "section.foo.bar.baz";
    // str2 = "20-KJ";
    // cout << "before 3th call: " << endl;
    // res = ini->Ini::set_value(str1, str2);
    // cout << "set_value return 3th call: " << res << endl;

    int serverSocket;
    int clientSocket;
    // creating socket
    cout << "  before socket " << endl;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) // SOCK_STREAM = TCP
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // INADDR_ANY;

    // if (serverSocket)
    // {
    //     close(serverSocket);
    // }
    cout << "  bind" << endl;
    // binding socket.
    if (bind(serverSocket, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listening to the assigned socket
    if (listen(serverSocket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        // accepting connection request
        if ((clientSocket = accept(serverSocket, nullptr, nullptr)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // recieving data
        char buffer[1024] = {0};
        recv(clientSocket, buffer, sizeof(buffer), 0);
        cout << "Message from client: " << buffer
             << endl;

        string str = buffer;
        char *resp;
        int pos;
        if (str.find("--load") != std::string::npos)
        {
            pos = str.find("--load");
            cout << "str: " << str << endl;
            cout << "str.find(--load): " << pos << endl;

            str = str.substr(pos + strlen("--load"));
            str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
            cout << "load path: " << str << endl;

            res = ini->Ini::load_resurce(str);
            cout << "load_resurce return: " << res << endl;
            if (res == 0)
            {
                cout << "res == 0" << endl;
                resp = "0\n";
            }
            else
            {
                cout << "load_resurce return: " << res << endl;
                resp = "127\n";
            }
            cout << "before send msg to client: " << res << endl;
            send(clientSocket, resp, strlen(resp), 0);
            cout << "Message sent LOAD: " << str << endl;

            cout << endl;
        }
        else if (str.find("--get") != std::string::npos)
        {
            pos = str.find("--get");
            cout << "str: " << str << endl;
            str = str.substr(pos + strlen("--get"));
            string buff;
            cout << "key: " << str << endl;
            str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
            res = ini->Ini::get_value(str, buff);
            cout << "get_value return: " << buff << endl;

            if (res == 0)
            {
                resp = buff.data();
            }
            else
            {
                resp = "127\n";
            }
            send(clientSocket, resp, strlen(resp), 0);
            cout << "Message sent GET: " << str << endl;
            cout << endl;
        }
        else if (str.find("--set") != std::string::npos) //./client –set section.color.red “roses are red”
        {
            pos = str.find("--set");
            cout << "--set  pos   : " << pos << endl;

            // std::cout << "is ASCII: " << std::boolalpha << isASCII(str) << "\n";
            // pos = str.find("\xe2\x86\x92");
            // cout << "non ASCI    pos   : " << pos << endl;
            //  string tester = "\"";
            str = str.substr(pos + strlen("--set")); //, str.find(tester)

            char *ch = str.data();
            int n_quotes = 0;
            string str1;
            string str2;

            // void stripUnicode(string & str)
            // {
            //     str.erase(remove_if(str.begin(), str.end(), invalidChar), str.end());
            // }
            string str3, str4;
            for (int i = 0; i < strlen(ch); i++)
            {
                if (invalidChar(ch[i]))
                {
                    n_quotes++;
                }
                else if (!invalidChar(ch[i]) && n_quotes == 0)
                {
                    cout << "find \" ch[i] put into str1 " << ch[i] << endl;
                    str1.push_back(ch[i]);
                }
                else if (!invalidChar(ch[i]) && n_quotes > 0)
                {
                    cout << "find \" ch[i] put into str2 " << ch[i] << endl;
                    str2.push_back(ch[i]);
                }
                // else if (ch[i] != '\"' && n_quotes == 1)
                // {
                //     cout << "ch[i]: " << ch[i] << endl;
                //     str2.push_back(ch[i]);
                // }
                // else
                // {
                //     n_quotes++;
                // }
            }
            cout << "str1: " << str1 << endl;
            // pos = str1.find("roses are red"); // detect double quotes
            // cout << "\"  pos   : " << pos << endl;
            // str2 = str1.substr(pos);
            // str2 = str.substr(str.find_first_of("+"), str.find_last_of("+"));
            //  str2.erase(std::remove_if(str2.begin(), str2.end(), "\""), str2.end());

            // str2 = str.substr(pos + 1, str.size() - 2); //+1 and -2 are not a "magic numbers", it are just to delete the ""

            cout << "str1: " << str1 << endl;
            cout << "str2: " << str2 << endl;
            res = ini->Ini::set_value(str1, str2);
            cout << "set_value return: " << res << endl;

            if (res == 0)
            {
                resp = "0\n";
            }
            else
            {
                resp = "127\n";
            }
            send(clientSocket, resp, strlen(resp), 0);
            cout << "Message sent SET: " << str << endl;
            cout << endl;
        }
        else
        {
            // unknow command
            resp = "127\n";
            send(clientSocket, resp, strlen(resp), 0);
            printf("Message sent 127\n");
            cout << endl;
        }
    }

    // closing the socket.
    close(serverSocket);

    // res =

    // load_resurce(path);

    // delete ini;

    // if(!path(path))
    // {

    //     throw domain_error("Error path is not defined.");
    // }

    return EXIT_SUCCESS;
}

// int main(int argc, char *argv[])
// {

//    std::cout << "hello" << std::endl;

//     return EXIT_SUCCESS;
// }