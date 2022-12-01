#include "p2pserver.h"
#include "server.h"
#include "pthread.h"
#include "request.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
typedef void * (*THREADFUNCPTR)(void *);

void * p2pserver::execute(void * req){
    char buffer[512];
    server.tryRecvMessage(buffer,0,server.getClientFd());
    cout << "Server received: " << buffer << endl;
    return NULL;
}
p2pserver::p2pserver(int port){
   //server server(port);
}

void p2pserver::run(){
    //server server(port);
    if(server.getErrorCode() == -1 || server.tryListen() == -1){
        cerr<<"Error to listen on the port"<<endl;
        exit(EXIT_FAILURE);
    }
    //build deamon
    cout << "Waiting for connection on port " << port << endl;
    while(true){
        // Accept one connection in the queue
        if(server.tryAccept() == -1){
            cerr<<"Error to listen on the port"<<endl;
            continue;
        }
        // Create a request
        request * req = new request(server.getClientFd(), server.getClientIp());
        // Create a new thread to handle the request
        pthread_t thread;
        pthread_create(&thread, nullptr, (THREADFUNCPTR) & p2pserver::execute,req);
    }
}



