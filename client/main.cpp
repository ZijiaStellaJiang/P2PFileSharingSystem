#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include "client.h"
using namespace std;

int main(int argc, char *argv[]){
    const int port    = 3333;
    const char *hostname = argv[1];
    client client(hostname,port);
    cout << "connect " << port << endl;
    char message[] = "hi there! I'm client";
    int fd=client.getSocketFd();
    client.trySendMessage(message, fd);
    char buffer[512];
    client.tryRecvMessage(buffer,0,fd);
    cout << "Client received: " << buffer << endl;
    client.close();
    return 0;


}