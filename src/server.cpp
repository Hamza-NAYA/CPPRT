#include <iostream>
#include <winsock2.h>

int main() {
    // Initialize WSA variables
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if (wsaerr != 0) {
        std::cout << "The Winsock dll not found!" << std::endl;
        return 0;
    } else {
        std::cout << "The Winsock dll found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
    }

    // Create a socket
    SOCKET serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    } else {
        std::cout << "Socket is OK!" << std::endl;
    }

    // Bind the socket
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with your desired IP address
    service.sin_port = htons(55555);  // Choose a port number

    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
        std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    } else {
        std::cout << "bind() is OK!" << std::endl;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "listen() is OK! I'm waiting for new connections..." << std::endl;
    }

    // Accept incoming connections
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, nullptr, nullptr);

    if (acceptSocket == INVALID_SOCKET) {
        std::cout << "accept failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    } else {
        std::cout << "accept() is OK!" << std::endl;
    }

    // Receive data from the client
    char receiveBuffer[200];
    int rbyteCount = recv(acceptSocket, receiveBuffer, 200, 0);
    if (rbyteCount < 0) {
        std::cout << "Server recv error: " << WSAGetLastError() << std::endl;
        return 0;
    } else {
        std::cout << "Received data: " << receiveBuffer << std::endl;
    }

    // Send a response to the client
    char buffer[200];
    std::cout << "Enter the message: ";
    std::cin.getline(buffer, 200);
    int sbyteCount = send(acceptSocket, buffer, 200, 0);
    if (sbyteCount == SOCKET_ERROR) {
        std::cout << "Server send error: " << WSAGetLastError() << std::endl;
        return -1;
    } else {
        std::cout << "Server: Sent " << sbyteCount << " bytes" << std::endl;
    }

    // Clean up
    closesocket(acceptSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
