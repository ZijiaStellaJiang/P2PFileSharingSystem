#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include "server.h"
#include  "p2pserver.h"
using namespace std;

int main(int argc, char *argv[]){
    //int port     = 3333;
    //server server(port);
    //server.tryListen();
    //cout << "Waiting for connection on port " << port << endl;
    //server.tryAccept();
    //int fd=server.getClientFd();
    //char buffer[512];
    //server.tryRecvMessage(buffer,0,fd);
    //cout << "Server received: " << buffer << endl;
    //char message[] = "hi there! I'm server";
    //server.trySendMessage(message, fd);
    //server.close();
    //return 0;

    p2pserver pserver;
    pserver.run(3333);
    return 0;

}

