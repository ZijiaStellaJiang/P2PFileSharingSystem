#include "p2pClient.h"

int main(int argc, char *argv[]) {
    cout << "Welcome to P2P Network!\n" << endl;

    // check user commandline input
    if (argc < 2) {
        cout << "Please enter the server IP address to continue use Network."
             << endl;
        cout << "Exiting... " << endl;
        exit(-1);
    }
    const char *ipAddr = argv[1];
    p2pClient mainClient;
    mainClient.create_folder();
    mainClient.myClient.setclient(ipAddr, 3333);

    cout << "Please enter a port number for sharing your files: " << endl;
    int peerPort;
    cin >> peerPort;

    mainClient.pServer.setup(peerPort);
    thread thrd(&peerServer::run, &mainClient.pServer);
    thrd.detach();

    mainClient.run(peerPort);
}