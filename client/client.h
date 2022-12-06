#ifndef CLIENT_H
#define CLIENT_H
#define MAX_TCP_LEN 65535
#include "../server/protocol/server_peer.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/message_lite.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "buffer.h"
using namespace std;
using namespace google::protobuf::io;
class client {

    private:
        int socket_fd;
        int errorCode;
        struct addrinfo hit;
        struct addrinfo *res; // ll
        const char * ipAddr;
        int port;

    public:
        client(const char * ipAddr, int port);
        int getSocketFd() const;
        int getErrorCode() const;
        static int trySendMessage(char *message, int fd);
        static int tryRecvMessage(char *message, int mode, int fd);    // Try to receive the message on the port
        static int recvMessage(int fd, void * message, int length);
        void close();

        template<typename T>
        int recvMesg(int socket_fd, T& message);

        template<typename T>
        void resMesg(int socket_fd, const T& message);

    protected:
        void initHit(); // init the hit
        int getAddress(); // get the address
        int createSocket(); // Create a fd_socket
        int connectSocket(); // Connect to the socket
        int printError(std::string error) const; // Print Error
        static int sendall(int s, char *buf, int *len);

};


template<typename T>
int client::recvMesg(int socket_fd, T &message) {
    ssize_t curLen = 0;
    vector<char> buffer(1024);
    curLen = recv(socket_fd,buffer.data(), buffer.size(), 0);
    if (curLen < 0) {
        cerr<<"Error: Fail to receive response from the original server!"<<endl;
    }//else if(curLen == 0){
      //  return;
    //}
    //get the actual size of the buffer
    uint32_t contentSize = buffer::getContentLength(buffer);
    uint32_t totalSize = 1+contentSize;

    buffer.resize(totalSize);
    while(curLen < totalSize){
        int restLen = totalSize-curLen;
        int tmpLen = recv(socket_fd, &(buffer.data()[curLen]), restLen, 0);
        cout << tmpLen << endl;
        if(tmpLen < 0){
            cerr<<"Error: Fail to receive response from the server!"<<endl;
            return -1;
        }
        curLen += tmpLen;
    }
    google::protobuf::io::ArrayInputStream in(buffer.data(), buffer.size());
    google::protobuf::io::CodedInputStream input(&in);
    uint32_t size;
    if (!input.ReadVarint32(&size)) {
        cerr<<"Error: Fail to get the size of the received message!"<<endl;
        return -1;
    }
    // Tell the stream not to read beyond that size.
    google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit(size);
    message.ParseFromCodedStream(&input);
    // Release the limit.
    input.PopLimit(limit);
    return 1;
}

template<typename T>
void client::resMesg(int socket_fd, const T& message){
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
    int status = send(socket_fd, buffer.data(), buffer.size(), 0);
    if(status == -1){
        cerr<<"Error: Fail to send request content to the original server!"<<endl;
    }    
}

#endif