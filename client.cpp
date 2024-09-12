// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

#include <filesystem>
// namespace fs = std::filesystem;

using namespace std;

int main(int argc, char *argv[])
{
    // creating socket
    int clientSocket;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // sending connection request
    if (connect(clientSocket, (struct sockaddr *)&serverAddress,
                sizeof(serverAddress)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    int send_to_server;
    // int pos;
    string str;
    string path;
    // sending data
    for (int i = 0; i < argc; i++)
    {

        char *message = argv[i];
        // string str = argv[i];
        // cout << endl;
        // cout << "str: " << str << endl;
        // if (str.find("--load"))
        // {
        // path = str.substr(str.find("--load") + strlen("--load"));
        // cout << "load path: " << str << endl;

        // if (fs::exists(path))
        // {

        send_to_server = send(clientSocket, message, strlen(message), 0);
        if (send_to_server == -1)
        {
            perror("\nSend to server error \n");
            return -1;
        }

        // for (int i = 0; i < strlen(message); i++)
        // {
        //     cout << "" << message[i];
        // }
        // cout << endl;
        //  }
        // }
    }
    char server_response[1024] = {0};
    recv(clientSocket, server_response, sizeof(server_response), 0);

    cout << endl;
    cout << " Client recive: " << server_response << endl;
    // closing socket
    close(clientSocket);

    return 0;
}

//       if (pos = str.find("--load"))
//     {
//         str = str.substr(pos + strlen("--load"));
//         cout << "load path: " << str << endl;

//     }
//     else if (pos = str.find("--get"))
//     {
//         str = str.substr(pos + strlen("--get"));

//     }
//     else if (pos = str.find("--set"))
//     {
//         str = str.substr(pos + strlen("--set"));
//         pos = str.find('"');
//         string str1 = str.substr(0, pos);
//         string str2 = str.substr(pos, str.size() - 2); // -2 is not a "magic number", it is just to delete the '"'

//     }
//     else
//     {
//         // unknow command

//     }

//     std::filesystem::path p = "foo.c";
// std::cout << "Current path is " << std::filesystem::current_path() << '\n';
// std::cout << "Absolute path for " << p << " is " << fs::absolute(p) << '\n';