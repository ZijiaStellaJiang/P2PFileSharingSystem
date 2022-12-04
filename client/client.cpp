#include "client.h"


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