#ifndef SERVER_H
#define SERVER_H
#define MAX_TCP_LEN 65535  // max TCP size
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/message_lite.h>

#include "protocol/server_peer.pb.h"
using namespace google::protobuf::io;

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "buffer.h"

using namespace std;

class server {
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
    server(){};
    ~server(){};
    int getClientFd() const;

    const std::string &getClientIp() const;

    void setServer(int port);

    int tryListen();

    int tryAccept();

    static int trySendMessage(std::string message, int fd);

    static int tryRecvMessage(char *message, int mode, int fd);

    void close();

    int getPort();

   protected:
    void initHit();  // init the hit

    int getAddress();  // get the address

    int createSocket();  // Create a fd_socket

    int bindPort();  // bind the port with socket

    int listen();  // listen the port

    int accept();  // accept one in queue and return the new fd

    static int printError(std::string error);

   public:
    template <typename T>
    int recvMesg(int client_fd, T &message);

    template <typename T>
    void resMesg(int client_fd, const T &message);
};
template <typename T>
int server::recvMesg(int client_fd, T &message) {
    ssize_t curLen = 0;
    vector<char> buffer(1024);
    curLen = recv(client_fd, buffer.data(), buffer.size(), 0);
    if (curLen < 0) {
        cerr << "Error: Fail to receive response from the original server!"
             << endl;
    }  // else if(curLen == 0){
       //  return;
    //}
    // get the actual size of the buffer
    uint32_t contentSize = buffer::getContentLength(buffer);
    uint32_t totalSize = 1 + contentSize;

    buffer.resize(totalSize);
    while (curLen < totalSize) {
        int restLen = totalSize - curLen;
        int tmpLen = recv(client_fd, &(buffer.data()[curLen]), restLen, 0);
        cout << tmpLen << endl;
        if (tmpLen < 0) {
            cerr << "Error: Fail to receive response from the server!" << endl;
            return -1;
        }
        curLen += tmpLen;
    }
    google::protobuf::io::ArrayInputStream in(buffer.data(), buffer.size());
    google::protobuf::io::CodedInputStream input(&in);
    uint32_t size;
    if (!input.ReadVarint32(&size)) {
        cerr << "Error: Fail to get the size of the received message!" << endl;
        return -1;
    }
    // Tell the stream not to read beyond that size.
    google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit(size);
    message.ParseFromCodedStream(&input);
    // Release the limit.
    input.PopLimit(limit);
    return 1;
}
template <typename T>
void server::resMesg(int client_fd, const T &message) {
    // get size of message
    const size_t contentSize = message.ByteSizeLong();
    const size_t headSize = to_string(contentSize).length();
    const size_t totalSize = headSize + contentSize;
    vector<char> buffer(totalSize);
    google::protobuf::io::ArrayOutputStream out(buffer.data(), totalSize);
    google::protobuf::io::CodedOutputStream output(&out);
    // Write the size
    output.WriteVarint32(contentSize);
    message.SerializeToCodedStream(&output);
    int status = send(client_fd, buffer.data(), buffer.size(), 0);
    if (status == -1) {
        cerr << "Error: Fail to send request content to the original server!"
             << endl;
    }
}

#endif