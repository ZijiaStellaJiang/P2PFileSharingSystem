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
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include "protocol/server_peer.pb.h"
#include "database.h"

using namespace std;

class p2pserver{
    private:
    static server serverr;
    ServerData p2pDatabase;

    public:
    p2pserver(){};
    ~p2pserver(){};
    void run();
    static void * execute(void * req);

    void handleRequest(const clientRequest &clientRequest, int user_ip);
    serverResp handleShare(const C2SShare &c2sShare, int user_ip);
    serverResp handleQuery(const C2SQuery &c2sQuery,int user_ip);
    serverResp handleDelete(const C2SDelete &c2sDelete,int user_ip);
    serverResp handleQuit(const C2SQuit &c2sQuit,int user_ip);
};


#endif
