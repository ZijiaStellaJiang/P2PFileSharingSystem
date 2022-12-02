#include <pqxx/pqxx>
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
#include "protocol/server_peer.pb.h"

using namespace std;
using namespace pqxx;
using std::exception;

class ServerData{
    private:
        shared_ptr<connection> C;
    public:
        mutex mtx;
        ServerData();
        ~ServerData();
        void createTables();
        void DropAllTables();
        int add_file(int user_id, int user_ip, string file_name, int file_size, int file_ttl, int port);
        S2CQuery query_file(string file_name);
        int delete_file(string file_name);
        int delete_user(int user_id);
};



// int add_user(connection *C, int user_ip);
// int is_user(connection *C, int user_id, int user_ip);



