#include "server.h"

#include <arpa/inet.h>

#include <fstream>

void server::setServer(int port) {
    this->errorCode = 0;
    this->port = port;
    initHit();
    if (getAddress() == -1) {
        this->errorCode = -1;  // get addinfo
        return;
    }
    if (createSocket() == -1) {
        this->errorCode = -1;  // Create socket
        return;
    }
    if (bindPort() == -1) {
        this->errorCode = -1;  // bind the port
        return;
    }
}

void server::initHit() {
    memset(&this->hit, 0, sizeof(this->hit));
    this->hit.ai_family = AF_UNSPEC;
    this->hit.ai_socktype = SOCK_STREAM;
    this->hit.ai_flags = AI_PASSIVE;
}

int server::getAddress() {
    if (getaddrinfo(nullptr, std::to_string(this->port).c_str(), &this->hit,
                    &this->res) != 0)
        return printError("Error: cannot get address info for host");
    return 0;
}
int server::printError(std::string error) {
    std::cerr << "(no-id):  " << error << std::endl;
    return -1;
}

int server::createSocket() {
    this->socket_fd = socket(this->res->ai_family, this->res->ai_socktype,
                             this->res->ai_protocol);
    if (this->socket_fd == -1) printError("Error: cannot create socket");
    return this->socket_fd;
}

int server::bindPort() {
    int yes = 1;
    int status = setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                            sizeof(int));
    status = ::bind(this->socket_fd, this->res->ai_addr, this->res->ai_addrlen);
    if (status == -1) return printError("Error: cannot bind socket");
    freeaddrinfo(this->res);
    return 0;
}

int server::listen() {
    if (::listen(this->socket_fd, 100) == -1)
        return printError("Error: cannot listen on socket");
    return 0;
}

int server::tryListen() { return listen(); }

/* get sockaddr */
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int server::accept() {
    char s[INET6_ADDRSTRLEN];
    struct sockaddr_storage their_addr;
    socklen_t socket_addr_len = sizeof(their_addr);
    int client_connection_fd;

    client_connection_fd =
        ::accept(socket_fd, (struct sockaddr *)&their_addr, &socket_addr_len);

    if (client_connection_fd == -1) {
        return printError("Error: cannot accept connection on socket");
    }
    // Record the player's connection_fd
    this->clientFd = (client_connection_fd);

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              s, sizeof s);

    // Record player's ip address
    struct sockaddr_in *addr = (struct sockaddr_in *)&their_addr;
    this->clientIp = inet_ntoa(addr->sin_addr);
    return client_connection_fd;
}

void server::close() { ::close(this->socket_fd); }

int server::tryRecvMessage(char *message, int mode, int fd) {
    int numbytes = 0;
    if (-1 == (numbytes = recv(fd, message, MAX_TCP_LEN - 1, mode))) {
        perror("recv");
        return -1;
    }
    message[numbytes] = '\0';
    return numbytes;
}

int server::trySendMessage(std::string message, int fd) {
    if (send(fd, message.c_str(), message.length(), 0)) return -1;
    return 0;
}

int server::tryAccept() { return this->clientFd = accept(); }

int server::getErrorCode() const { return errorCode; }

int server::getClientFd() const { return clientFd; }

const std::string &server::getClientIp() const { return clientIp; }

int server::getPort() { return port; }
