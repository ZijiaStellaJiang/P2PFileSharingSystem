#include "p2pserver.h"
#include "server.h"
#include "pthread.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
typedef void * (*THREADFUNCPTR)(void *);

void * p2pserver::execute(server &server)
{
    int fd=server.getClientFd();
    char buffer[512];
    server.tryRecvMessage(buffer,0,fd);
    cout << "Server received: " << buffer << endl;
    return NULL;
}

void p2pserver::run(int port){
    server server(port);
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
        // Create a new thread to handle the request
        pthread_t thread;
        pthread_create(&thread, nullptr, (THREADFUNCPTR) &p2pserver::execute,&server);
    }
}



