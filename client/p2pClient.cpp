#include "p2pClient.h"

void p2pClient::create_folder() {
    char curr_folder[512];
    getcwd(curr_folder, sizeof(curr_folder));

    // create a folder for this client
    string path = curr_folder;
    share_path = path + '/' + "Share";
    download_path = path + '/' + "Download";
    mkdir(share_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(download_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void p2pClient::run(int peer_port) {
    while (true) {
        string client_req;
        cout << endl;
        cout << "What do you need today? Please enter the number of the choice "
                "you "
                "like: \n 1. Share\n 2. Delete\n 3. Query\n 4. "
                "Quit\n"
             << endl;
        cin >> client_req;
        // string request = myClient.sendRequest();
        int err;
        if (client_req == "1") {
            err = handleShare(peer_port);
        } else if (client_req == "2") {
            handleDelete();
        } else if (client_req == "3") {
            handleQuery();

        } else if (client_req == "4") {
            err = handleQuit();
        } else {
            cout << "Please enter valid choice number." << endl;
            continue;
        }
        if (err) {
            continue;
        }
        serverResp serverResp;
        myClient.recvMesg(myClient.getSocketFd(), serverResp);
        handleResponse(serverResp);
    }
}

int p2pClient::handleShare(int peer_port) {
    clientRequest request;
    C2SShare *shareReq = new C2SShare();
    cout << endl;
    cout << "Reminder: Please add the file you would like to share in the "
            "folder ./Share"
         << endl;
    cout << "The file NOT in the ./Share folder will NOT be shared" << endl;
    cout << endl;
    cout << "Please enter the number of files you want to share (min 1): "
         << endl;
    int time;
    cin >> time;
    shareReq->set_port(peer_port);
    int i = 1;

    while (i <= time) {
        cout << "File " << i << " :" << endl;
        cout << "Please enter the File name:" << endl;
        string filename;
        cin >> filename;
        bool isFile = is_file(filename);
        if (!isFile) {
            cout << "The filename you entered is not in the ./Share folder, "
                    "please add the file first."
                 << endl;
            return 1;
        }
        cout << "Please enter the File size:" << endl;
        int size;
        cin >> size;
        cout << "Please enter the number of person you would like to share "
                "with: "
             << endl;
        int ttl;
        cin >> ttl;
        fileInfo *fileinfo = shareReq->add_file_info();
        setReq(fileinfo, filename, size, ttl);
        i++;
    }
    request.set_allocated_req_share(shareReq);
    myClient.resMesg(myClient.socket_fd, request);
    return 0;
}

bool p2pClient::is_file(string file_name) {
    string file_path = share_path + "/" + file_name;
    ifstream file;
    file.open(file_path);
    if (file) {
        file.close();
        return true;
    } else {
        return false;
    }
}

void p2pClient::setReq(fileInfo *fileReq, string file_name, int file_size,
                       int file_ttl) {
    fileReq->set_file_name(file_name);
    fileReq->set_file_size(file_size);
    fileReq->set_file_ttl(file_ttl);
}

void p2pClient::handleDelete() {
    clientRequest request;
    C2SDelete *deleteReq = new C2SDelete();
    cout << "Please enter the number of files you want to delete: " << endl;
    int time;
    cin >> time;
    int i = 1;
    while (i <= time) {
        cout << "File " << i << " :" << endl;
        cout << "Please input the File name you want to delete: " << endl;
        string name;
        cin >> name;
        string *filename = deleteReq->add_file_name();
        *filename = name;
        i++;
    }
    request.set_allocated_req_delete(deleteReq);
    myClient.resMesg(myClient.socket_fd, request);
}

void p2pClient::handleQuery() {
    clientRequest request;
    C2SQuery *queryReq = new C2SQuery();
    cout << "Please enter the name of file you want to Query: " << endl;
    string line;
    cin >> line;
    string filename = line;
    queryReq->set_file_name(filename);
    request.set_allocated_req_query(queryReq);
    myClient.resMesg(myClient.socket_fd, request);
}

int p2pClient::handleQuit() {
    clientRequest request;
    C2SQuit *quitReq = new C2SQuit();
    cout << "Are you sure to quit the Network? \n1.Yes\n2.No" << endl;
    int num;
    cin >> num;
    if (num != 1) {
        return 1;
    }
    quitReq->set_request_quit(num);
    request.set_allocated_req_quit(quitReq);
    myClient.resMesg(myClient.socket_fd, request);
    return 0;
}

void p2pClient::handleResponse(const serverResp &serverResp) {
    if (serverResp.has_resp_share()) {
        S2CShare share = serverResp.resp_share();
        cout << endl;
        cout << "Share Result:\n Number of shared files: " << share.resp_size()
             << endl;
        for (int i = 0; i < share.resp_size(); i++) {
            fileNameResponse fileResp = share.resp()[i];
            if (fileResp.resp()) {
                cout << i + 1 << ". File name: " << fileResp.file_name()
                     << " Result: Succeed " << endl;
            } else {
                cout << i + 1 << ". File name: " << fileResp.file_name()
                     << " Result: Failed (Duplicate file already Shared) "
                     << endl;
            }
        }
    }
    if (serverResp.has_resp_query()) {
        S2CQuery query = serverResp.resp_query();
        if (query.resp() && query.resp()) {
            int res = downloadfile(query);
            string file_name = query.file_name();
            if (!res) {
                cout << "Query Result: Failed, File not exist. " << endl;
            } else {
                cout << "Succesefully download the file : " << file_name
                     << endl;
                cout << "Please visit ./Download folder." << endl;
            }
        } else {
            cout << "Query Result: Failed, File not exist. " << endl;
        }
    }
    if (serverResp.has_resp_delete()) {
        S2CDelete del = serverResp.resp_delete();
        cout << endl;
        cout << "Delete Result: \nNumber of delete file: " << del.resp_size()
             << endl;
        for (int i = 0; i < del.resp_size(); i++) {
            fileNameResponse file = del.resp(i);

            if (file.resp()) {
                cout << i + 1 << ". File Name: " << file.file_name()
                     << "Result: Succeed " << endl;
            } else {
                cout << i + 1 << "delete . File Name: " << file.file_name()
                     << " Result: Failed (Out of TTL) " << endl;
            }
        }
    }
    if (serverResp.has_resp_quit()) {
        S2CQuit quit = serverResp.resp_quit();
        if (quit.resp()) {
            cout << endl;
            cout << "Thank you for using P2P Network!" << endl;
            cout << "Exiting... " << endl;
            // close the p2p client;
            close();
        } else {
            cout << "Quit Failed, please use Ctrl+C to quit. " << endl;
        }
    }
}

void p2pClient::close() {
    myClient.close();
    pServer.close();
    pClient.close();
    exit(1);
}

int p2pClient::downloadfile(S2CQuery query_info) {
    // string file_name = query_info.file_name();
    // int file_size = query_info.file_size();
    // string target_ip = query_info.target_ip();
    // int target_port = query_info.target_port();

    // TO ZIJIA: Please add the socket part here:
    // const char *ip = target_ip.c_str();
    // pClient.setclient(ip, target_port);
    // 1. Set up the client (I changed the client little bit so, use
    // setclient(ip,port))

    // example: (not sure, havn't test yet)
    // const char *ip = target_ip.c_str();
    // pClient.setclient(ip, target_port);

    // 2. receive the file from other user's peerServer
    // 3. Save the received content in the ./Download folder with the file_name.
    // 4. Return 1 if success. 0 if not.
    return 1;
}