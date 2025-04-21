#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <limits>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080

//initialize winsock
void initializeWinsock()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        exit(1);
    }
}

SOCKET connectToServer(const std::string &serverAddress, int port)
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    }

    sockaddr_in serverInfo;
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(port);
    inet_pton(AF_INET, serverAddress.c_str(), &serverInfo.sin_addr);

    if (connect(sock, (sockaddr *)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR)
    {
        std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        exit(1);
    }

    return sock;
}

void handleServerResponse(SOCKET clientSocket)
{
    char buffer[1024];
    int valread;
    std::string message;

    while (true)
    {
        valread = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (valread > 0)
        {
            buffer[valread] = '\0';
            std::cout << "Server: " << buffer << std::endl;
            // Check if the server is asking for further input
            if (std::string(buffer).find("Enter") != std::string::npos)
            {
                std::cout << "Client: ";
                std::getline(std::cin, message);
                send(clientSocket, message.c_str(), static_cast<int>(message.length()), 0);
            }
            else
            {
                while ((valread = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0)
                {
                    buffer[valread] = '\0';
                    std::cout << "Server: " << buffer << std::endl;
                    if (std::string(buffer).find("Operation completed.") != std::string::npos)
                    {
                        break;
                    }
                }
                break;
            }
        }
        else
        {
            std::cerr << "Failed to receive data from server. Error: " << WSAGetLastError() << std::endl;
            break;
        }
    }
}

int main()
{
    initializeWinsock();
    SOCKET clientSocket = connectToServer("127.0.0.1", PORT); // IP Address

    std::cout << "Connected to server\n";

    while (true)
    {
        std::cout << "Please select your choice: " << "\n";
        std::cout << "1. Display all Books\n";
        std::cout << "2. Search for a book\n";
        std::cout << "3. Borrow a book\n";
        std::cout << "4. Return a book\n";

        std::string message;
        std::cin >> message;

        // Clear the input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Send the initial choice to the server
        send(clientSocket, message.c_str(), static_cast<int>(message.length()), 0);

        // Handle server response
        handleServerResponse(clientSocket);
    }

    // Close the socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}