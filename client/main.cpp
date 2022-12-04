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
#include <thread>
#include "client.h"
#include "peerServer.h"
#include "peerClient.h"
using namespace std;

int main(int argc, char *argv[]){
    const int port    = 3333;
    const char * ipAddr = argv[1];
    const int peerPort = atoi(argv[2]);
    // this peerPort should later inform p2pserver
    client client(ipAddr,port);

    // connect with p2p server
    cout << "connect " << port << endl;
    char message[] = "hi there! I'm client";
    int fd=client.getSocketFd();
    client.trySendMessage(message, fd);

    // open a peer server socket
    peerServer prServer(peerPort);
    thread thrd(&peerServer::run, &prServer);
    thrd.detach();

    // connect with the peer server for sharing file
    // try connection here
    cout << "test pass through" << endl;
    // place holder, should use request type later for condition
    if (argc == 5) {
        const int sharePort = atoi(argv[3]);
        // this sharePort should be passed as a parameter later from the S2CQuery
        peerClient prClient(ipAddr, sharePort);
        char * sharemsg = argv[4];
        int prSvFd = prClient.getSocketFd();
        prClient.trySendMessage(sharemsg, prSvFd);
    }

    while(true) {
        // do the request and response here
    }

    return 0;


}