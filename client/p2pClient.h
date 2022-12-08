#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "client.h"
#include "peerClient.h"
#include "peerServer.h"

class p2pClient {
   public:
    p2pClient(){};

    string share_path;
    string download_path;
    void run(int peer_port);

    client myClient;
    peerClient pClient;
    peerServer pServer;

    ~p2pClient(){};

    void create_folder();

    void run();

    int handleShare(int peer_port);
    void setReq(fileInfo *fileReq, string file_name, int file_size,
                int file_ttl);
    void handleDelete();
    void handleQuery();
    bool is_file(string file_name);
    int handleQuit();
    void handleResponse(const serverResp &serverResp);
    int downloadfile(S2CQuery query_info);
    void close();
};