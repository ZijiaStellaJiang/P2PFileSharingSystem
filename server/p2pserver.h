#ifndef P2PSERVER_H
#define P2PSERVER_H
#define MAX_TCP_LEN 65535  // max TCP size

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "database.h"
#include "protocol/server_peer.pb.h"
#include "pthread.h"
#include "request.h"
#include "server.h"

using namespace std;

class p2pserver {
   private:
    server serverr;
    ServerData p2pDatabase;

    // request handler

   public:
    p2pserver(){};
    ~p2pserver(){};
    void run();

    void executeThread(request *req);

    void sendResponse(const serverResp &serverResp, int user_fd,
                      string user_ip);
    int handleRequest(const clientRequest &clientRequest, string user_ip,
                      int user_fd);
    void handleShare(const C2SShare &c2sShare, string user_ip, int user_fd);
    void handleQuery(const C2SQuery &c2sQuery, int user_fd, string user_ip);
    void handleDelete(const C2SDelete &c2sDelete, int user_fd, string user_ip);
    int handleQuit(const C2SQuit &c2sQuit, string user_ip, int user_fd);
    void setResult(fileNameResponse *fileResp, int res, string file_name);
};

#endif
