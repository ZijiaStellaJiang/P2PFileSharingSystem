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
#include "server.h"
#include "pthread.h"
#include "request.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class p2pserver{
    private:
    static server serverr;

    public:
    p2pserver(){};
    ~p2pserver(){};
    void run();
    static void * execute(void * req);

    ;
};


#endif
