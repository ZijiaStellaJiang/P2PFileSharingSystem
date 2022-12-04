#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <fstream>
#include <mutex>
#include <ctime>
#include <unordered_map>
#include <exception>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include "protocol/server_peer.pb.h"
#include "request.h"

using namespace std;
using std::exception;
using namespace google::protobuf;

Message * handleRequest(void * request);

void handleShare(const C2SShare &c2sShare);
void handleQuery(const C2SQuery &c2sQuery);
void handleDelete(const C2SDelete &c2sDelete);
void handleQuit(const C2SQuit &c2sQuit);