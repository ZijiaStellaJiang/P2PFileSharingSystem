#ifndef SERVER_H
#define SERVER_H
#define MAX_TCP_LEN 65535 // max TCP size

#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>


class server{
    public:
    int getErrorCode() const;
private:
    int errorCode;
    int socket_fd;
    struct addrinfo hit;
    struct addrinfo *res;
    int port;
    int clientFd;
    std::string clientIp;

public:
    int getClientFd() const;
    
    const std::string &getClientIp() const;

    server(int port);

    int tryListen();

    int tryAccept();

    static int trySendMessage(std::string message, int fd);

    static int tryRecvMessage(char *message, int mode, int fd);

    void close();

    int getPort();
    protected:
    void initHit(); // init the hit

    int getAddress(); // get the address

    int createSocket(); // Create a fd_socket

    int bindPort(); // bind the port with socket

    int listen(); // listen the port

    int accept(); // accept one in queue and return the new fd

    static int printError(std::string error) ;


};

#endif