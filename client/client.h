#ifndef CLIENT_H
#define CLIENT_H
#define MAX_TCP_LEN 65535
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>

class client {

    private:
        int socket_fd;
        int errorCode;
        struct addrinfo hit;
        struct addrinfo *res; // ll
        const char *hostname;
        int port;
    public:
        client(const char * hostname, int port);
        int getSocketFd() const;
        int getErrorCode() const;
        static int trySendMessage(char *message, int fd);
        static int tryRecvMessage(char *message, int mode, int fd);    // Try to receive the message on the port
        static int recvMessage(int fd, void * message, int length);
        void close();

    protected:
        void initHit(); // init the hit
        int getAddress(); // get the address
        int createSocket(); // Create a fd_socket
        int connectSocket(); // Connect to the socket
        int printError(std::string error) const; // Print Error
        static int sendall(int s, char *buf, int *len);


};

#endif