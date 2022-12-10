#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "p2pserver.h"
#include "server.h"
using namespace std;

int main(int argc, char *argv[]) {
    p2pserver pserver;
    pserver.run();
    return 1;
}
