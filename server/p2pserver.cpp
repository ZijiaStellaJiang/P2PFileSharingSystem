#include "p2pserver.h"
#include "server.h"
#include "pthread.h"
#include "request.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
typedef void * (*THREADFUNCPTR)(void *);
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
server p2pserver::serverr = server(3333);

void * p2pserver::execute(void * req){
    pthread_mutex_lock(&mutex_lock);
    char buffer[512];
    serverr.tryRecvMessage(buffer,0,serverr.getClientFd());
    // req.getFd()
    cout << "Server received: " << buffer << endl;
    pthread_mutex_unlock(&mutex_lock);
    return nullptr;
}

void p2pserver::run(){
    
    if(serverr.getErrorCode() == -1 || serverr.tryListen() == -1){
        cerr<<"Error to listen on the port"<<endl;
        exit(EXIT_FAILURE);
    }
    //build deamon
    int port=serverr.getPort();
    serverr.tryListen();
    cout << "Waiting for connection on port " << port << endl;
    while(true){
        // Accept one connection in the queue
        if(serverr.tryAccept() == -1){
            cerr<<"Error to accept on the port"<<endl;
            continue;
        }
        // Create a request
        request * req = new request(serverr.getClientFd(), serverr.getClientIp());
        // Create a new thread to handle the request
        pthread_t thread;
        pthread_create(&thread, nullptr, (THREADFUNCPTR) & p2pserver::execute,req);
        
    }
}



