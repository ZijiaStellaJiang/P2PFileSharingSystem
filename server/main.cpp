#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include "server.h"
using namespace std;

int main(int argc, char *argv[]){
    int port     = 4444;
    server server(port);
    server.tryListen();
    cout << "Waiting for connection on port " << port << endl;
    server.tryAccept();
    server.close();
    return 0;


}

