#ifndef PEERCLIENT_H
#define PEERCLIENT_H
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

#include "client.h"

class peerClient : public client {
   public:
    peerClient(){};
    ~peerClient(){};
    // peerClient(const char* ipAddr, int port);
};

#endif