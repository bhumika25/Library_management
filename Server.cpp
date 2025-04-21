#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include "Library.h"

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
Library library;

// initialize winsock
void initializeWinsock() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        exit(1);
    }
}

// search book
void handleSearch(SOCKET clientSock) {
    char buffer[1024];
    std::string keyword, type;
    Book* book = nullptr;

    const char* message = "Enter Keyword to Search: ";
    if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSock);
        return;
    }

    int recievedData = recv(clientSock, buffer, sizeof(buffer), 0);
    if (recievedData > 0) {
        buffer[recievedData] = '\0';
        keyword = buffer;
    }

    message = "Enter type of keyword (id or name or author): \n";
    if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSock);
        return;
    }

    recievedData = recv(clientSock, buffer, sizeof(buffer), 0);
    if (recievedData > 0) {
        buffer[recievedData] = '\0';
        type = buffer;
    }

    book = library.searchBook(keyword, type);
    if (book) {
        message = "Book found:\n";
        if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            closesocket(clientSock);
            return;
        }
        std::string bookDetails = book->displayBooks();
        int sendResult = send(clientSock, bookDetails.c_str(), static_cast<int>(bookDetails.length()), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            closesocket(clientSock);
            return;
        }
    } else {
        message = "Book not found.\n";
        if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            closesocket(clientSock);
            return;
        }
    }

    // Send completion message
    message = "Operation completed.";
    if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSock);
        return;
    }
}

// Borrow book
void handleBorrow(SOCKET clientSock) {
    char buffer[1024];
    int id, book_id;

    const char* message = "Enter User ID: ";
    if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSock);
        return;
    }
    std::cout << "Sent message: " << message << std::endl;

    int recievedData = recv(clientSock, buffer, sizeof(buffer), 0);
    if (recievedData > 0) {
        buffer[recievedData] = '\0';
        id = std::stoi(buffer); // Convert the received string to an integer
        std::cout << "Received User ID: " << id << std::endl;
    }

    message = "Enter Book ID: ";
    if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSock);
        return;
    }
    std::cout << "Sent message: " << message << std::endl;

    recievedData = recv(clientSock, buffer, sizeof(buffer), 0);
    if (recievedData > 0) {
        buffer[recievedData] = '\0';
        book_id = std::stoi(buffer); // Convert the received string to an integer
        std::cout << "Received Book ID: " << book_id << std::endl;
    }

    library.addTransaction(id, book_id, "Borrowed", clientSock);

    // Send completion message
    message = "Operation completed.\n";
    send(clientSock, message, static_cast<int>(strlen(message)), 0);
}

// Display books
void handleDisplay(SOCKET clientSock) {
    library.displayBook(clientSock);
    const char* message = "Operation completed.\n";
    send(clientSock, message, static_cast<int>(strlen(message)), 0);
}

// Return a book
void handleReturn(SOCKET clientSock) {
    char buffer[1024];
    int id, book_id;

    const char* message = "Enter User ID: ";
    if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSock);
        return;
    }
    std::cout << "Sent message: " << message << std::endl;

    int recievedData = recv(clientSock, buffer, sizeof(buffer), 0);
    if (recievedData > 0) {
        buffer[recievedData] = '\0';
        id = std::stoi(buffer); // Convert the received string to an integer
        std::cout << "Received User ID: " << id << std::endl;
    }

    message = "Enter Book ID: ";
    if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSock);
        return;
    }
    std::cout << "Sent message: " << message << std::endl;

    recievedData = recv(clientSock, buffer, sizeof(buffer), 0);
    if (recievedData > 0) {
        buffer[recievedData] = '\0';
        book_id = std::stoi(buffer); // Convert the received string to an integer
        std::cout << "Received Book ID: " << book_id << std::endl;
    }

    library.addTransaction(id, book_id, "Returned", clientSock);

    // Send completion message
    message = "Operation completed.\n";
    send(clientSock, message, static_cast<int>(strlen(message)), 0);
}

void handleClient(SOCKET clientSock) {
    char buffer[1024];
    int recievedData;

    while ((recievedData = recv(clientSock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[recievedData] = '\0';
        std::string command(buffer);

        if (command == "1") {

            handleDisplay(clientSock);
        } else if (command == "2") {
            handleSearch(clientSock);
        } else if (command == "3") {
            handleBorrow(clientSock);
        } else if (command == "4") {
            handleReturn(clientSock);
        } else {
            const char* message = "Unknown command!";
            if (send(clientSock, message, static_cast<int>(strlen(message)), 0) == SOCKET_ERROR) {
                std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
                closesocket(clientSock);
                return;
            }
        }
    }

    closesocket(clientSock);
}

int main() {
    initializeWinsock();

    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        SOCKET clientSock = accept(serverSock, nullptr, nullptr);
        if (clientSock == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            closesocket(serverSock);
            WSACleanup();
            return 1;
        }

        std::thread clientThread(handleClient, clientSock);
        clientThread.detach();
    }

    closesocket(serverSock);
    WSACleanup();
    return 0;
}