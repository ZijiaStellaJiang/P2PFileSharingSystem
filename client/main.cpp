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
#include "../server/protocol/server_peer.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/message_lite.h>
#include "client.h"
#include "peerServer.h"
#include "peerClient.h"
#include "buffer.h"
using namespace std;

string createFolder(const char* client_name);

/**
 * argv should be like this pattern:
 * ./main [server's ip address]
*/
int main(int argc, char *argv[]){
    const char * ipAddr = argv[1];
    client client(ipAddr, 3333);
    string clientpath = createFolder(ipAddr);

    // open a thread to run peer server socket
    cout << "Please decide a port for sharing your file: " << endl;
    int peerPort;
    cin >> peerPort;
    peerServer prServer(peerPort);
    thread thrd(&peerServer::run, &prServer);
    thrd.detach();

    // interact with client, deal with its requests
    while(true) {
        string request = client.sendRequest();
        if (request == "share") {
            client.handleShare(peerPort, clientpath);
        }
        else if (request == "delete") {
            client.handleDelete();
        }
        else if (request == "query") {
            client.handleQuery();

        }
        else if (request == "quit") {
            client.handleQuit();
        }
        serverResp serverResp;
        client.recvMesg(client.getSocketFd(), serverResp);
        client.handleResponse(serverResp);
    }


    // if (argc == 6) {
    //     const int sharePort = atoi(argv[4]);
    //     peerClient prClient(ipAddr, sharePort);
    //     char * sharemsg = argv[5];
    //     int prSvFd = prClient.getSocketFd();
    //     prClient.trySendMessage(sharemsg, prSvFd);
    //     char buffer[512];
    //     prClient.tryRecvMessage(buffer, 0, prClient.getSocketFd());
    //     cout << "my peer send me file with filename: " << buffer << endl;
    // }


    // // if the client wants to quit, then delete the folder
    // rmdir((clientpath).c_str());

    // // close the client

    return 0;
}

/**
 * this function create share and download folder for this client
 * the path will be currentpath/clientFile/client_name/(share or download)
 * return the path of this client's folder: currentpath/clientFile/client_name
 * this folder will be deleted if the client choose to quit p2p network
*/
string createFolder(const char* client_name) {
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