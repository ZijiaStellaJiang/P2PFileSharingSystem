#ifndef P2PSERVER_H
#define P2PSERVER_H
#define MAX_TCP_LEN 65535 // max TCP size

#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include "server.h"
#include <map>
#include <string>
using namespace std;

class p2pserver{
    private:
    int port;
    public:
    p2pserver();
    public:
    void run(int port);
    void * execute(server &server);

};


#endif
