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
    const int port    = 4444;
    const char *hostname = argv[1];
    client client(hostname,port);
    cout << "connect " << port << endl;
    client.close();
    return 0;


}