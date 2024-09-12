
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

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
    string str;
    string path;

    for (int i = 0; i < argc; i++)
    {
        char *message = argv[i];
        send_to_server = send(clientSocket, message, strlen(message), 0);
        if (send_to_server == -1)
        {
            perror("\nSend to server error \n");
            return -1;
        }
    }
    char server_response[1024] = {0};
    recv(clientSocket, server_response, sizeof(server_response), 0);

    cout << endl;
    cout << " Client recive: " << server_response << endl;

    close(clientSocket);
    return 0;
}
