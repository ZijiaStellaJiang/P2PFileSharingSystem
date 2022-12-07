#include "client.h"
#include <fstream>
#include <iostream>

client::client(const char * ipAddr, int port) {
    this->errorCode = 0;
    this->port = port;
    this->ipAddr = ipAddr;
    initHit();
    if(getAddress() == -1){
        this->errorCode = -1;
        return;
    }
    if(createSocket() == -1){
        this->errorCode = -1;
        return;
    }
    if(connectSocket() == -1){
        this->errorCode = -1;
        return;
    }
    freeaddrinfo(this->res);
}

void client::initHit() {
    memset(&this->hit, 0, sizeof(this->hit));
    this->hit.ai_family = AF_UNSPEC;
    this->hit.ai_socktype = SOCK_STREAM;
}

int client::getAddress() {
    if(getaddrinfo(this->ipAddr, std::to_string(this->port).c_str(), &this->hit, &this->res) != 0) return printError("Error: cannot get address info for host");
    return 0;
}

int client::createSocket() {
    this->socket_fd = socket(this->res->ai_family, this->res->ai_socktype, this->res->ai_protocol);
    if(this->socket_fd == -1) return printError("Error: cannot create socket");
    return 0;
}

int client::connectSocket() {
    if (connect(this->socket_fd, this->res->ai_addr, this->res->ai_addrlen) == -1) return printError("Error: cannot connect to socket");
    return 0;
}

int client::printError(std::string error) const {
    std::cerr << "(no-id):  " <<  error << std::endl;
    return -1;
}

int client::tryRecvMessage(char *message, int mode, int fd) {
    int numbytes = 0;
    if ((numbytes = recv(fd, message, MAX_TCP_LEN - 1, mode)) == -1) {
        perror("recv");
        return -1;
    }
    message[numbytes] = '\0';
    return numbytes;
}

int client::trySendMessage(char message[MAX_TCP_LEN], int fd) {
    int len;
    len = strlen(message);
    if (sendall(fd, message, &len) == -1) {
        perror("sendall");
        printf("We only sent %d bytes because of the error!\n", len);
        return -1;
    }
    return 0;
}

int client::sendall(int s, char *buf, int *len)
{
    int total = 0;
    int bytesleft = *len;
    int n;

    while(total < *len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    *len = total;

    return n==-1?-1:0;
}


void client::close() {
    ::close(this->socket_fd);
}

int client::getSocketFd() const {
    return socket_fd;
}

int client::getErrorCode() const {
    return errorCode;
}

int client::recvMessage(int fd, void *message, int length) {
    return recv(fd, message, length, 0);
}

void client::setReq(fileInfo * fileReq, string file_name,int file_size, int file_ttl){
    fileReq->set_file_name(file_name);
    fileReq->set_file_size(file_size);
    fileReq->set_file_ttl(file_ttl);
}

void client::handleShare(int sharePort, string path){
    clientRequest request;
    C2SShare * shareReq = new C2SShare();
    cout<<"please input the number of file you want to share: "<<endl;
    int time;
    cin>>time;
    // cout<<"please input the port you want to share the file"<<endl;
    // int port;
    // cin>>port;
    shareReq->set_port(sharePort);
    while(time-->0){
        fileInfo* fileinfo=shareReq->add_file_info();
        cout<<"please input the File name you want to share"<<endl;
        string filename;
        cin>>filename;
        cout<<"please input the File size you want to share"<<endl;
        int size;
        cin>>size;
        cout<<"please input the ttl "<<endl;
        int ttl;
        cin>>ttl;
        setReq(fileinfo,filename,size,ttl);

        // add to client's folder
        string filePath(path);
        filePath = filePath + '/' + "share/" + filename + ".txt";
        fstream fs (filePath, fstream::in | fstream::out);
        fs.close();
    }
    request.set_allocated_req_share(shareReq);
    resMesg(socket_fd,request);
}
void client::handleDelete(){
    clientRequest request;
    C2SDelete * deleteReq = new C2SDelete();
    cout<<"please input the number of file you want to delete: "<<endl;
    int time;
    cin>>time;
    while(time-->0){
        cout<<"please input the File name you want to delete"<<endl;
        string name;
        cin>>name;
        string* filename=deleteReq->add_file_name();
        *filename=name;
    }
    request.set_allocated_req_delete(deleteReq);
    resMesg(socket_fd,request);
}
void client::handleQuery(){
    clientRequest request;
    C2SQuery * queryReq = new C2SQuery();
    cout<<"please input the name of file you want to Query: "<<endl;
    string line;
    cin>>line;
    string filename=line;
    queryReq->set_allocated_file_name(&filename);
    request.set_allocated_req_query(queryReq);
    resMesg(socket_fd,request);
}
void client::handleQuit(){
    clientRequest request;
    C2SQuit * quitReq = new C2SQuit();
    cout<<"please make sure if you want to quit the netowrk "<<endl;
    int num;
    cin>>num;
    quitReq->set_request_quit(num);
    request.set_allocated_req_quit(quitReq);
    resMesg(socket_fd,request);
}
//method to send server request;
string client::sendRequest(){
    string type;
    cout<<" what do you want to do? \n 1. share\n 2. delete\n 3. query\n 4. quit\n"<<endl;
    getline(cin,type);
    return type;
    // if(type=="share"){
    //     handleShare();
    // }
    // if(type=="delete"){
    //    handleDelete();
    // }
    // if(type=="query"){
    //     handleQuery();
    // }
    // if(type=="quit"){
    //   handleQuit();
    // }
}
//method to analyze the server response
void client::handleResponse(const serverResp& serverResp){
     if(serverResp.has_resp_share()){
        S2CShare share = serverResp.resp_share();
        cout<<"Share, number of share files "<< share.resp_size()<<endl;
        for(int i=0;i<share.resp_size();i++){
            fileNameResponse fileResp = share.resp()[i];
            if(fileResp.resp()){
                cout<<i+1<<". File name: " << fileResp.file_name()<< " Result: "<<" succeed "<<endl;
            }else{
                cout<<i+1<<". File name: " << fileResp.file_name()<< " Result: "<<" failed "<<endl;
            }
        }
    }
    if(serverResp.has_resp_query()){
        S2CQuery query = serverResp.resp_query();
        if(query.resp()){
            cout<<"Result: "<<" Query succeed "<<endl;
            cout<<"File name: "<<query.file_name()<<endl;
            cout<<"File size: "<<query.file_size()<<endl;
            cout<<"Target ip: "<<query.target_ip()<<endl;
            cout<<"Target port: "<<query.target_port()<<endl;
        }else{
            cout<<"Result: "<<" Query failed "<<endl;
        }
        // start the peer client and ask for file from the target client
    }
    if(serverResp.has_resp_delete()){
        S2CDelete del = serverResp.resp_delete();
        cout<<"Delete "<<endl;
        for(int i=0;i<del.resp_size();i++){
            fileNameResponse file = del.resp(i);
            if(file.resp()){
                cout<<i+1<<"delete . File Name: "<<file.file_name()<<" succeed "<<endl;
            }else{
                cout<<i+1<<"delete . File Name: "<<file.file_name()<<" failed "<<endl;
            }
        }
    }
    if(serverResp.has_resp_quit()){
        S2CQuit quit = serverResp.resp_quit();
        if(quit.resp()){
           cout<<" Quit "<<endl;
           //close the p2p client;
        }else{
           cout<<" Quit cancelled "<<endl;
        }
    }
}