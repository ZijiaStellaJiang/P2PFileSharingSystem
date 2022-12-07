#ifndef PEERCLIENT_H
#define PEERCLIENT_H
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include "client.h"

class peerClient : public client{

    public:
        peerClient(const char * ipAddr, int port);
                
};

#endif