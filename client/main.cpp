#include <netdb.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <thread>
#include "client.h"
#include "peerServer.h"
#include "peerClient.h"
using namespace std;

void connectp2pServer(client & client, char* name);
string createFolder(char* client_name);

int main(int argc, char *argv[]){
    const char * ipAddr = argv[1];
    int peerPort = atoi(argv[2]);
    char * clientName = argv[3];
    client client(ipAddr,3333);
    connectp2pServer(client, clientName);
    string clientpath = createFolder(clientName);

    // open a thread to run peer server socket
    peerServer prServer(peerPort);
    thread thrd(&peerServer::run, &prServer);
    thrd.detach();

    // connect with the peer server for sharing file
    // try connection here
    cout << "test pass through" << endl;
    // place holder, should use request type later for condition
    if (argc == 6) {
        const int sharePort = atoi(argv[4]);
        // this sharePort should be passed as a parameter later from the S2CQuery
        peerClient prClient(ipAddr, sharePort);
        char * sharemsg = argv[5];
        int prSvFd = prClient.getSocketFd();
        prClient.trySendMessage(sharemsg, prSvFd);
        char buffer[512];
        prClient.tryRecvMessage(buffer, 0, prClient.getSocketFd());
        cout << "my peer send me file with filename: " << buffer << endl;
    }

    while(true) {
        // do the request and response here
    }

    // if the client wants to quit, then delete the folder
    rmdir((clientpath).c_str());

    // close the client

    return 0;


}

void connectp2pServer(client & client, char* name) {
    cout << "Connected to p2p server on port 3333" << endl;
    string message(name);
    message += " joins the p2p network.";
    int fd = client.getSocketFd();
    client.trySendMessage(const_cast<char*>(message.c_str()), fd);
}

/**
 * this function create share and download folder for this client
 * the path will be currentpath/clientFile/client_name/(share or download)
 * return the path of this client's folder: currentpath/clientFile/client_name
 * this folder will be deleted if the client choose to quit p2p network
*/
string createFolder(char* client_name) {
    // get the current path of this client
    char curr_folder[512];
    getcwd(curr_folder, sizeof(curr_folder));

    // create a folder for this client
    string path = curr_folder;
    path = path + '/' + "clientFile/" + client_name;
    string clientpath(path);
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    // create share and download folders for this client
    string sharepath = path + '/' + "share";
    string downloadpath = path + '/' + "download";
    mkdir(sharepath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(downloadpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    return clientpath;
}