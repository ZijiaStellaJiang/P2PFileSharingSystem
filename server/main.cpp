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
#include  "p2pserver.h"
using namespace std;

int main(int argc, char *argv[]){
    p2pserver pserver;
    pserver.run();
    return 0;
}

