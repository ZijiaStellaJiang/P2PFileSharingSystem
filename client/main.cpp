#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <stdlib.h>
#include "client.h"
#include "peerServer.h"
using namespace std;

int main(int argc, char *argv[]){
    const int port    = 3333;
    const char *hostname = argv[1];
    const int peerPort = atoi(argv[2]);
    client client(hostname,port);

    // connect with p2p server
    cout << "connect " << port << endl;
    char message[] = "hi there! I'm client";
    int fd=client.getSocketFd();
    client.trySendMessage(message, fd);

    // open a peer server socket
    peerServer prServer(peerPort);
    prServer.run();

    char buffer[512];
    client.tryRecvMessage(buffer,0,fd);
    cout << "Client received: " << buffer << endl;

    client.close();
    return 0;


}