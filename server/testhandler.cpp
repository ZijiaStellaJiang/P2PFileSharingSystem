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
using namespace google::protobuf;

int main(){
    p2pserver pserver;
    pserver.run();

    int user_ip1 = 1234567890;
    int user_ip2 = 1987654321;
    

    //Test Share 1 file:
    clientRequest * S1request = new clientRequest();
    C2SShare * share = new C2SShare();
    fileInfo * info = share->add_file_info();
    string file_name = "Share_example";
    int file_size = 12;
    int file_ttl = 50;
    info->set_file_name(file_name);
    info->set_file_size(file_size);
    info->set_file_ttl(file_ttl);   
    share->set_port(12345);
    S1request->set_allocated_req_share(share);

    //Test Share 2 files:
    clientRequest * S2request = new clientRequest();
    C2SShare * share2 = new C2SShare();
    share2->set_port(12345);
    fileInfo * info1 = share2->add_file_info();
    fileInfo * info2 = share2->add_file_info();
    
    string file_name1 = "Share_example1";
    int file_size1 = 12;
    int file_ttl1 = 50;
    info1->set_file_name(file_name1);
    info1->set_file_size(file_size1);
    info1->set_file_ttl(file_ttl1);   

    string file_name2 = "Share_example2";
    int file_size2 = 32;
    int file_ttl2 = 150;
    info2->set_file_name(file_name2);
    info2->set_file_size(file_size2);
    info2->set_file_ttl(file_ttl2);   
    
    S2request->set_allocated_req_share(share2);

    //Test Query
    clientRequest * Qrequest = new clientRequest();
    C2SQuery * q = new C2SQuery();
    string q_name = "Share_example";
    q->set_file_name(q_name);
    Qrequest->set_allocated_req_query(q);

    //Test Delete for not exist
    clientRequest *D1request = new clientRequest();
    C2SDelete *d1 = new C2SDelete();
    string d1_name = "not_exist_filename";
    d1->add_file_name(d1_name);
    D1request->set_allocated_req_delete(d1);

    //Test Delete for 2
    clientRequest *D2request = new clientRequest();
    C2SDelete *d2 = new C2SDelete();
    string d2_name1 = "Share_example";
    string d2_name2 = "Share_example1";
    d2->add_file_name(d2_name1);
    d2->add_file_name(d2_name2);
    D2request->set_allocated_req_delete(d2);

    //test Quit
    clientRequest *QuitRequest = new clientRequest();
    C2SQuit *quit = new C2SQuit();
    quit->set_request_quit(1);
    QuitRequest->set_allocated_req_quit(quit);

    pserver.handleRequest(*S1request,user_ip1);
    pserver.handleRequest(*S2request,user_ip2);
    pserver.handleRequest(*Qrequest,user_ip1);
    // pserver.handleRequest(*D1request,user_ip1);
    // pserver.handleRequest(*D2request,user_ip1);
    pserver.handleRequest(*QuitRequest,user_ip2);
}