#include "Server.h"

Server::Server(unsigned short port) {
    this->_port = port;
    #ifdef _WIN32
        WSADATA wsa_data;
        WSAStartup(MAKEWORD(1, 1), &wsa_data);
    #endif
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!IS_SOCKET_VALID(this->_serverSocket)) {
        closesocket(this->_serverSocket);
        throw std::runtime_error("Networking Error: server socket creation failed");
    }
}

Server::~Server() {
    #ifdef _WIN32
        WSACleanup();
    #endif
}

void Server::start() {
    struct sockaddr_in serv_addr {};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->_port);
    if (bind(this->_serverSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        throw std::runtime_error("Networking Error: server socket binding failed");
    }
}

void Server::waitForClient() {
    struct sockaddr_in cli_addr {};
    socklen_t clilen = sizeof(cli_addr);
    printf("listening for clients\n");
    listen(this->_serverSocket,5);
    this->_clientSocket = accept(this->_serverSocket,
                       (struct sockaddr *) &cli_addr, &clilen);
    printf("client found\n");
    if (!IS_SOCKET_VALID(this->_clientSocket)) {
        throw std::runtime_error("Networking Error: client socket acceptance failed");
    }
}

int Server::closeSocket(SOCKET sock) {
    int status = 0;
    #ifdef _WIN32
        status = shutdown(sock, SD_BOTH);
        if (status == 0) { status = closesocket(sock); }
    #else
        status = shutdown(sock, SHUT_RDWR);
        if (status == 0) { status = close(sock); }
    #endif
    return status;
}

void Server::sendMessage(std::string message) {
    send(this->_clientSocket, message.c_str(), static_cast<int>(message.length()), 0);
}

