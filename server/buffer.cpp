#include "buffer.h"
#include "protocol/server_peer.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
using namespace google::protobuf::io;

uint32_t buffer::getWholeBufferLength(vector<char> buffer) {
    uint32_t contentSize = getContentLength(buffer);
    return std::to_string(contentSize).length()+contentSize;
}

uint32_t buffer::getContentLength(vector<char> buffer) {
    google::protobuf::io::ArrayInputStream in(buffer.data(), buffer.size());
    google::protobuf::io::CodedInputStream input(&in);
    uint32_t contentSize;
    if(!input.ReadVarint32(&contentSize)){
        cerr<< "Error: Fail to get the size of the received message!"<<endl;
    }
    return contentSize;
}
