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
    this->_running = false;
    closeSocket(this->_serverSocket);
    closeSocket(this->_clientSocket);
    #ifdef _WIN32
        WSACleanup();
    #endif
        if(this->_clientAcceptorThread.joinable()){
            this->_clientAcceptorThread.detach();
        }
}

void Server::start() {
    this->_running = true;
    struct sockaddr_in serv_addr {};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->_port);
    if (bind(this->_serverSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        throw std::runtime_error("Networking Error: server socket binding failed");
    }
    this->_clientAcceptorThread = std::thread(&Server::acceptClients, this);
}

void Server::acceptClients() {
    while(this->_running) {
        struct sockaddr_in cli_addr{};
        socklen_t clilen = sizeof(cli_addr);
        listen(this->_serverSocket, 5);
        SOCKET temp = accept(this->_serverSocket,
                                     (struct sockaddr *) &cli_addr, &clilen);
        if(IS_SOCKET_VALID(this->_clientSocket)){
            this->closeSocket(this->_clientSocket);
        }
        this->_clientSocket = temp;
        if (!IS_SOCKET_VALID(this->_clientSocket)) {
            throw std::runtime_error("Networking Error: client socket acceptance failed");
        }
        printf("client found\n");
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
    if(send(this->_clientSocket, message.c_str(), static_cast<int>(message.length()), 0) == SOCKET_ERROR){
        throw std::runtime_error("Networking Error: sending data failed");
    }
}

Command Server::recvMessage() {
    char buf[3];
    recv(this->_clientSocket, buf, sizeof(buf), 0);
    return Command {buf[0] == VALIDATION_CHAR,buf[1],buf[2]};
}

