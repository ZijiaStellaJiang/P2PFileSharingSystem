#ifdef HOSTCLIENT_H
#define HOSTCLIENT_H

#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>

class hostClient {
    private:
        int socket_fd;
        int error_code;
        struct addrinfo hit;
        struct addrinfo *res;
        int port;
        int peer_fd;
};

#endif