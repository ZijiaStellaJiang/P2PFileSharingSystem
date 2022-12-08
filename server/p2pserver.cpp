#include "p2pserver.h"

using namespace std;
typedef void *(*THREADFUNCPTR)(void *);
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
server p2pserver::serverr = server(3333);

void p2pserver::run() {
    cout << "Start Running P2P Directory Server.." << endl;
    cout << " " << endl;
    if (serverr.getErrorCode() == -1 || serverr.tryListen() == -1) {
        cerr << "Error to listen on the port" << endl;
        exit(EXIT_FAILURE);
    }
    // build deamon
    int port = serverr.getPort();
    cout << "Waiting for connection on port " << port << endl;
    while (true) {
        // Accept one connection in the queue
        if (serverr.tryAccept() == -1) {
            cerr << "Error to accept on the port" << endl;
            continue;
        }
        // Create a request
        request *req =
            new request(serverr.getClientFd(), serverr.getClientIp());
        thread thd(&p2pserver::executeThread, this, ref(req));
        thd.detach();
    }
}

void p2pserver::executeThread(request *req) {
    while(true) {
        clientRequest clientReq;
        int res = serverr.recvMesg(req->getFd(), clientReq);
        if (res == 1) {
            handleRequest(clientReq, req->getIpAddress(), req->getFd());
        }
    }
    
}

void p2pserver::sendResponse(const serverResp &serverResp, int user_fd) {
    cout << " " << endl;
    serverr.resMesg(user_fd, serverResp);

    cout << "Send Server Response" << endl;
    cout << "Send Server Response" << endl;
    cout << "Send Server Response" << endl;
    cout << "Send Server Response" << endl;
    cout << "Send Server Response" << endl;
    cout << "Send Server Response" << endl;
    cout << "Send Server Response" << endl;
    cout << "Send Server Response" << endl;
    // if (serverResp.has_err()) {
    //     cout << "error request" << endl;
    // }
    // if (serverResp.has_resp_share()) {
    //     S2CShare share = serverResp.resp_share();
    //     cout << "Share, number of share files " << share.resp_size() << endl;
    //     for (int i = 0; i < share.resp_size(); i++) {
    //         fileNameResponse fileResp = share.resp()[i];
    //         cout << i + 1 << ". File name: " << fileResp.file_name()
    //              << " Result: " << fileResp.resp() << endl;
    //     }
    // }
    // if (serverResp.has_resp_query()) {
    //     S2CQuery query = serverResp.resp_query();
    //     cout << "Query " << endl;
    //     cout << "Result: " << query.resp() << endl;
    //     cout << "File name: " << query.file_name() << endl;
    //     cout << "File size: " << query.file_size() << endl;
    //     cout << "Target ip: " << query.target_ip() << endl;
    //     cout << "Target port: " << query.target_port() << endl;
    // }
    // if (serverResp.has_resp_delete()) {
    //     S2CDelete del = serverResp.resp_delete();
    //     cout << "Delete " << endl;
    //     for (int i = 0; i < del.resp_size(); i++) {
    //         fileNameResponse file = del.resp(i);
    //         cout << i + 1 << ". File Name: " << file.file_name()
    //              << " Result: " << file.resp() << endl;
    //     }
    // }
}

void p2pserver::handleRequest(const clientRequest &clientRequest,
                              string user_ip, int user_fd) {
    // received share request
    serverResp response;
    // error check
    if (clientRequest.has_req_share() + clientRequest.has_req_query() +
            clientRequest.has_req_delete() + clientRequest.has_req_quit() !=
        1) {
        response.set_err(true);
        sendResponse(response, user_fd);
    }

    // find the request type
    // received share
    if (clientRequest.has_req_share()) {
        C2SShare req_share = clientRequest.req_share();
        cout << "Received Share Request from " << user_ip << endl;
        handleShare(req_share, user_ip, user_fd);
    }
    // received query
    if (clientRequest.has_req_query()) {
        C2SQuery req_query = clientRequest.req_query();
        cout << "Received Query Request from " << user_ip << endl;
        // cout << "file name: " << req_query.file_name() << endl;
        handleQuery(req_query, user_fd);
    }
    // received delete
    if (clientRequest.has_req_delete()) {
        C2SDelete req_delete = clientRequest.req_delete();
        cout << "Received Delete Request from " << user_ip << endl;
        handleDelete(req_delete, user_fd);
    }
    // received quit
    if (clientRequest.has_req_quit()) {
        C2SQuit req_quit = clientRequest.req_quit();
        cout << "Received Quit Request from " << user_ip << endl;
        handleQuit(req_quit, user_ip, user_fd);
    }
}

void p2pserver::handleShare(const C2SShare &c2sShare, string user_ip,
                            int user_fd) {
    int num_files = c2sShare.file_info_size();
    int port = c2sShare.port();
    serverResp response;
    S2CShare *shareResp = new S2CShare();
    for (int i = 0; i < num_files; i++) {
        fileInfo file = c2sShare.file_info(i);
        string file_name = file.file_name();
        int file_size = file.file_size();
        int file_ttl = file.file_ttl();
        int res = this->p2pDatabase.add_file(file_name, file_size, file_ttl,
                                             user_ip, port);
        fileNameResponse *fileResp = shareResp->add_resp();
        setResult(fileResp, res, file_name);
    }
    response.set_allocated_resp_share(shareResp);
    response.set_err(false);
    sendResponse(response, user_fd);
}

void p2pserver::handleQuery(const C2SQuery &c2sQuery, int user_fd) {
    serverResp response;
    S2CQuery *queryResp = new S2CQuery();
    queryResp = this->p2pDatabase.query_file(c2sQuery.file_name());
    response.set_allocated_resp_query(queryResp);
    response.set_err(false);

    sendResponse(response, user_fd);
}

void p2pserver::handleDelete(const C2SDelete &c2sDelete, int user_fd) {
    int num_files = c2sDelete.file_name_size();
    serverResp response;
    S2CDelete *deleteResp = new S2CDelete();
    for (int i = 0; i < num_files; i++) {
        string file_name = c2sDelete.file_name(i);
        int res = this->p2pDatabase.delete_file(file_name);
        fileNameResponse *fileResp = deleteResp->add_resp();
        setResult(fileResp, res, file_name);
    }
    response.set_allocated_resp_delete(deleteResp);
    response.set_err(false);
    sendResponse(response, user_fd);
}

void p2pserver::handleQuit(const C2SQuit &c2sQuit, string user_ip,
                           int user_fd) {
    serverResp response;
    S2CQuit *quitResp = new S2CQuit();
    int res = this->p2pDatabase.delete_user(user_ip);
    if (!res) {
        quitResp->set_resp(false);
    } else {
        quitResp->set_resp(true);
    }
    response.set_allocated_resp_quit(quitResp);
    response.set_err(false);
    sendResponse(response, user_fd);
}

void p2pserver::setResult(fileNameResponse *fileResp, int res,
                          string file_name) {
    fileResp->set_file_name(file_name);
    if (!res) {
        fileResp->set_resp(false);
    } else {
        fileResp->set_resp(true);
    }
}