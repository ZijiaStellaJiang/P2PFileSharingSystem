#include "requestHandler.h"

Message * handleRequest(void * request){
    google::protobuf::Message * msg;

}

void handleShare(const C2SShare &c2sShare, int user_ip){
    int num_files = c2sShare.file_info_size();
    int port = c2sShare.port();
    for(int i=0;i<num_files; i++){
        fileInfo file = c2sShare.file_info()[i];
        string file_name = file.file_name();
        int file_size = file.file_size();
        int file_ttl = file.file_ttl();
                


    }
}
void handleQuery(const C2SQuery &c2sQuery);
void handleDelete(const C2SDelete &c2sDelete);
void handleQuit(const C2SQuit &c2sQuit);