#ifndef PEERSERVER_H
#define PEERSERVER_H

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "../server/server.h"

/**
 * this peerServer is the client who wants to share files
 * it acts as a server to other peers
 */
class peerServer {
   private:
    server serv;
    string share_path;

   public:
    peerServer(){};
    ~peerServer(){};

    void add_sharepath(string sharepath) { share_path = sharepath; }
    void run();
    void execute();
    void setup(int port);
    void close();
};

#endif