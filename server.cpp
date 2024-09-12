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
#include <arpa/inet.h> /

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
    // Ini *ini = new Ini();
    unsigned short res;
    int serverSocket;
    int clientSocket;

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
        cout << "Server wait for request..." << endl;
        cout << endl;
        // accepting connection request
        if ((clientSocket = accept(serverSocket, nullptr, nullptr)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // recieving data
        char buffer[1024] = {0};
        recv(clientSocket, buffer, sizeof(buffer), 0);
        cout << "Request: " << buffer<< endl;
cout << endl;

        string str = buffer;
        char *resp;
        int pos;
        if (str.find("--load") != std::string::npos)
        {
            pos = str.find("--load");
            str = str.substr(pos + strlen("--load"));
            str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
            res = load_resurce(str);
            cout << "Load resurce function return: " << res << endl;
            cout << endl;

            if (res == 0)
            {
                resp = "0\n";
            }
            else
            {
                resp = "127\n";
            }
            send(clientSocket, resp, strlen(resp), 0);
        }
        else if (str.find("--get") != std::string::npos)
        {
            pos = str.find("--get");
            str = str.substr(pos + strlen("--get"));
            string buff;
            str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
            res = get_value(str, buff);
            cout << "Get value function return: " << res << endl;
            cout << endl;

            if (res == 0)
            {
                resp = buff.data();
            }
            else
            {
                resp = "127\n";
            }
            send(clientSocket, resp, strlen(resp), 0);
        }
        else if (str.find("--set") != std::string::npos)
        {
            pos = str.find("--set");
            str = str.substr(pos + strlen("--set"));

            char *ch = str.data();
            int n_quotes = 0;
            string str1;
            string str2;
            for (int i = 0; i < strlen(ch); i++)
            {
                if (invalidChar(ch[i]))
                {
                    n_quotes++;
                }
                else if (!invalidChar(ch[i]) && n_quotes == 0)
                {
                    str1.push_back(ch[i]);
                }
                else if (!invalidChar(ch[i]) && n_quotes > 0)
                {
                    str2.push_back(ch[i]);
                }
            }

            res = set_value(str1, str2);
            cout << "Set value function return: " << res << endl;
            cout << endl;

            if (res == 0)
            {
                resp = "0\n";
            }
            else
            {
                resp = "127\n";
            }
            send(clientSocket, resp, strlen(resp), 0);
        }
        else
        {
            // unknow command
            resp = "127\n";
            send(clientSocket, resp, strlen(resp), 0);
            cout << endl;
        }
    }
    close(serverSocket);
    return EXIT_SUCCESS;
}