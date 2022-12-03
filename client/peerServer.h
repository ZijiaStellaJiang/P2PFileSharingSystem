#ifndef PEERSERVER_H
#define PEERSERVER_H

#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>

#include "../server/server.h"

/**
 * this peerServer is the client who wants to share files
 * it acts as a server to other peers 
*/
class peerServer {
    private:
        server serv;
    
    public:
        peerServer(int port): serv(server(port)) {}
        void run();
        void execute();
};

#endif