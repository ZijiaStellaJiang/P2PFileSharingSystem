#include "p2pserver.h"
#include "thread"
#include "protocol/server_peer.pb.h"

using namespace std;
typedef void * (*THREADFUNCPTR)(void *);
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
server p2pserver::serverr = server(3333);
void* p2pserver::execute(void * req){
    //pthread_mutex_lock(&mutex_lock);
    char buffer[512];
    serverr.tryRecvMessage(buffer,0,serverr.getClientFd());
    cout << buffer << endl;
    
    clientRequest clientRequest;
    int res=serverr.recvMesg(serverr.getClientFd(),clientRequest);
    if(res==1) cout<<"success"<<endl;
    if(res==-1) cout<<"fail"<<endl;
    //pthread_mutex_unlock(&mutex_lock);
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
        //thread th(execute);
        //th.detach();
        request * req = new request(serverr.getClientFd(), serverr.getClientIp());
        //Create a new thread to handle the request
        pthread_t thread;
        pthread_create(&thread, nullptr, (THREADFUNCPTR) & p2pserver::execute,req);
        
    }
}

