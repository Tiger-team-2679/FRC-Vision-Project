#pragma once
#include <stdexcept>
#include <thread>
#include <stdexcept>
#include "../processing/Command.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#define IS_SOCKET_VALID(SOCK) (SOCK != INVALID_SOCKET)
#else
#include <sys/socket.h>
#define SOCKET int
#define IS_SOCKET_VALID(SOCK) SOCK >= 0
#endif

/**
 * an incoming message will look like this:
 *  "<VALIDATION_CODE><TARGET CODE><RETURN_METHOD>"
 *  each parameter should be one char
 */

/**
 * an outgoing message will look like this:
 *  "<VALIDATION_CODE>;<TARGET CODE>;<DISTANCE>;<ANGLE>"
 *  target code should be char, all other are numbers
 */

/**
 * the validation code in both is just a stupid char to tell if a mmessage has began
 */

class Server {
public:
    Server(unsigned short port);
    ~Server();
    void start();
    void sendMessage(std::string message);
    Command recvMessage();
private:
    unsigned short _port;
    bool _running = false;
    SOCKET _serverSocket;
    SOCKET _clientSocket;
    std::thread _clientAcceptorThread;
    void acceptClients();
    int closeSocket(SOCKET sock);
};