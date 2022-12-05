#ifndef SERVER_H
#define SERVER_H
#define MAX_TCP_LEN 65535 // max TCP size

#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <vector>
using namespace std;


class server{
    public:
    int getErrorCode() const;
private:
    int errorCode;
    int socket_fd;
    struct addrinfo hit;
    struct addrinfo *res;
    int port;
    int clientFd;
    std::string clientIp;

public:
    int getClientFd() const;
    
    const std::string &getClientIp() const;

    server(int port);

    int tryListen();

    int tryAccept();

    static int trySendMessage(std::string message, int fd);

    static int tryRecvMessage(char *message, int mode, int fd);

    void close();

    int getPort();
    protected:
    void initHit(); // init the hit

    int getAddress(); // get the address

    int createSocket(); // Create a fd_socket

    int bindPort(); // bind the port with socket

    int listen(); // listen the port

    int accept(); // accept one in queue and return the new fd

    static int printError(std::string error) ;
    
    template<typename T>
    void recvMesg(vector<char>& buffer,int client_fd, T& message);

    template<typename T>
    void resMesg(int client_fd, const T& message);

};
template<typename T>
void recvMesg(vector<char>& buffer,int client_fd, T &message) {
    ssize_t curLen = 0;
    curLen = tryRecvMessage(&buffer,0,socket_fd);
    if (curLen < 0) {
        cerr<<"Error: Fail to receive response from the original server!")<<endl;
    }else if(curLen == 0){
        return;
    }
    //get the actual size of the buffer
    uint32_t contentSize = Buffer::getContentLength(buffer);
    uint32_t totalSize = 1+contentSize;

    buffer.resize(totalSize);
    while(curLen < totalSize){
        int restLen = totalSize-curLen;
        int tmpLen = tryRecvMessage(&buffer,0,client_fd);
        cout << tmpLen << endl;
        if(tmpLen < 0){
            cerr<<"Error: Fail to receive response from the server!")<<endl;
        }
        curLen += tmpLen;
    }
    google::protobuf::io::ArrayInputStream in(buffer.data(), buffer.size());
    google::protobuf::io::CodedInputStream input(&in);
    uint32_t size;
    if (!input.ReadVarint32(&size)) {
        cerr<<"Error: Fail to get the size of the received message!")<<endl;
    }
    // Tell the stream not to read beyond that size.
    google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit(size);
    message.ParseFromCodedStream(&input);
    // Release the limit.
    input.PopLimit(limit);
}
template<typename T>
void resMesg(int client_fd, const T& message){
     //get size of message
    const size_t contentSize = message.ByteSizeLong();
    const size_t headSize = to_string(contentSize).length();
    const size_t totalSize = headSize+contentSize;
    vector<char> buffer(totalSize);
    google::protobuf::io::ArrayOutputStream out(buffer.data(), totalSize);
    google::protobuf::io::CodedOutputStream output(&out);
    //Write the size
    output.WriteVarint32(contentSize);
    message.SerializeToCodedStream(&output);
    int status = trySendMessage(&buffer,client_fd);
    if(status == -1){
        cerr<<"Error: Fail to send request content to the original server!"<<endl;
    }    
}


#endif