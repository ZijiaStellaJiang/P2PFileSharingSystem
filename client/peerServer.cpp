#include "peerServer.h"

#include <stdio.h>
#include <stdlib.h>

#include <mutex>
#include <thread>

using namespace std;

mutex mtx;

void peerServer::setup(int port) { serv.setServer(port); }

void peerServer::execute() {
    // receive file name from peerclient
    char file_name[256];
    cout << "Server Break Point 1" << endl;
    serv.tryRecvMessage(file_name, 0, serv.getClientFd());
    cout << "Server Break Point 2" << endl;
    // char curr_path[512];
    // getcwd(curr_path, sizeof(curr_path));

    // find file in ./Share folder
    // string currpath(curr_path);
    string filename(file_name);
    string file_path = share_path + '/' + "Share/" + filename;
    fstream file;
    file.open(file_path, ios::binary | ios::in);

    // notify peer client whether the file exists
    cout << "Server Break Point 3" << endl;
    if (file) {
        cout << "Server Break Point 4" << endl;
        serv.trySendMessage("T", serv.getClientFd());
        cout << "Server Break Point 5" << endl;
    } else {
        cout << "Server Break Point 6" << endl;
        serv.trySendMessage("F", serv.getClientFd());
        cout << "Server Break Point 7" << endl;
        return;
    }

    file.seekg(0, ios::end);
    int len = file.tellg();
    file.seekg(0, ios::beg);
    char* file_content = new char[len];
    file.read(file_content, len);
    cout << "Server Break Point 8" << endl;
    serv.trySendMessage(file_content, serv.getClientFd());
    cout << "Server Break Point 9" << endl;

    file.close();
    delete[] file_content;
}

void peerServer::run() {
    if (serv.getErrorCode() == -1 || serv.tryListen() == -1) {
        cerr << "Error to listen on the port" << endl;
        exit(EXIT_FAILURE);
    }

    // serv.tryListen();

    while (true) {
        // Accept one connection in the queue
        if (serv.tryAccept() == -1) {
            cerr << "Error to accept on the port" << endl;
            continue;
        }
        thread thrd(&peerServer::execute, this);
        thrd.detach();
    }
}

void peerServer::close() { serv.close(); }