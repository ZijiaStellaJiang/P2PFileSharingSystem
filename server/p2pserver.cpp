#include "p2pserver.h"

using namespace std;
typedef void * (*THREADFUNCPTR)(void *);
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
server p2pserver::serverr = server(3333);
void * p2pserver::execute(void * req){
    pthread_mutex_lock(&mutex_lock);
    char buffer[512];
    serverr.tryRecvMessage(buffer,0,serverr.getClientFd());
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

void p2pserver::handleRequest(const clientRequest &clientRequest, int user_ip){
    //received share request
    serverResp response;
    if(clientRequest.has_req_share()){
        C2SShare req_share = clientRequest.req_share();
        response = p2pserver::handleShare(req_share,user_ip);
    }
    //received share query
    if(clientRequest.has_req_query()){
        C2SQuery req_query = clientRequest.req_query();
        response = p2pserver::handleQuery(req_query,user_ip);
    }
    //received share delete
    if(clientRequest.has_req_delete()){
        C2SDelete req_delete = clientRequest.req_delete();
        response = p2pserver::handleDelete(req_delete,user_ip);
    }
    //received share quit
    if(clientRequest.has_req_quit()){
        C2SQuit req_quit = clientRequest.req_quit();
        response = p2pserver::handleQuit(req_quit,user_ip);
    }
}

serverResp p2pserver::handleShare(const C2SShare &c2sShare, int user_ip){
    int num_files = c2sShare.file_info_size();
    int port = c2sShare.port();
    for(int i=0;i<num_files; i++){
        fileInfo file = c2sShare.file_info()[i];
        string file_name = file.file_name();
        int file_size = file.file_size();
        int file_ttl = file.file_ttl();
    }
}
serverResp p2pserver::handleQuery(const C2SQuery &c2sQuery,int user_ip){

}
serverResp p2pserver::handleDelete(const C2SDelete &c2sDelete,int user_ip){
    int num_files = c2sDelete.file_name_size();
    for(int i=0;i<num_files;i++){

    }
}
serverResp p2pserver::handleQuit(const C2SQuit &c2sQuit,int user_ip){
    
}
