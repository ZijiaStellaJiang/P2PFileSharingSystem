#include "peerServer.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;

void peerServer::execute() {
    mtx.lock();
    
    cout << "test thread " << serv.getPort() << endl;
    char buffer[512];
    serv.tryRecvMessage(buffer, 0, serv.getClientFd());
    cout << "peer server received: " << buffer << endl;

    // here accept user input, later should fetch from this peerserver's folder
    cout << "please provide the filename" << endl;
    char toShare[64];
    cin.getline(toShare, 64);
    serv.trySendMessage(toShare, serv.getClientFd());

    mtx.unlock();
}

void peerServer::run() {
    if(serv.getErrorCode() == -1 || serv.tryListen() == -1){
        cerr<<"Error to listen on the port"<<endl;
        exit(EXIT_FAILURE);
    }
    
    //int port=serv.getPort();
    serv.tryListen();
    //cout << "Waiting for connection on port for sharing " << port << endl;

    while(true){
        // Accept one connection in the queue
        if(serv.tryAccept() == -1){
            cerr<<"Error to accept on the port"<<endl;
            continue;
        }
        execute();
    }
}