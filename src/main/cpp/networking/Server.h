#pragma once
#include <stdexcept>
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

#define CARGO_TARGET_CODE 'c'
#define SHIP_TARGET_CODE 's'
#define NULL_TARGET_CODE 'n'

#define INFINITE_RETURN_METHOD 'u'
#define SINGLE_RETURN_METHOD 'o'
#define NO_RETURN_METHOD 'n'

#define VALIDATION_CHAR '~'

typedef struct Command{
    bool is_valid;
    char target;
    char method;
} Command;

/**
 * an incoming message will look like this:
 *  "<VALIDATION_CODE>;<TARGET CODE>;<RETURN_METHOD>"
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
    void waitForClient();
    void sendMessage(std::string message);
    Command recvMessage();
private:
    unsigned short _port;
    SOCKET _serverSocket;
    SOCKET _clientSocket;
    int closeSocket(SOCKET sock);
};