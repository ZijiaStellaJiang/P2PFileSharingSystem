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

    // Create the client which will connect to p2p server.
    const char *ipAddr = argv[1];
    p2pClient p2p;
    p2p.create_folder();
    p2p.myClient.setclient(ipAddr, 3333);

    int peerPort;
    while (true) {
        cout << "Please enter a port number for sharing your files: " << endl;
        cin >> peerPort;
        if (peerPort > 9999 || peerPort < 1000) {
            cout << endl;
            cout << "Please enter a valid port number." << endl;
            cout << endl;
        } else {
            break;
        }
    }

    p2p.pServer.setup(peerPort);
    thread thrd(&peerServer::run, &p2p.pServer);
    thrd.detach();

    p2p.run(peerPort);
}