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
    serv.tryRecvMessage(file_name, 0, serv.getClientFd());
    string filename(file_name);

    string file_path = share_path + '/' + filename;
    fstream file;
    file.open(file_path, ios::binary | ios::in);

    // notify peer client whether the file exists

    if (file) {
        serv.trySendMessage("T", serv.getClientFd());

    } else {
        serv.trySendMessage("F", serv.getClientFd());

        return;
    }

    file.seekg(0, ios::end);
    int len = file.tellg();
    file.seekg(0, ios::beg);
    char* file_content = new char[len];
    file.read(file_content, len);

    serv.trySendMessage(file_content, serv.getClientFd());

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