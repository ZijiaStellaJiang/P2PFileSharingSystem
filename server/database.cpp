#include "database.h"


ServerData::ServerData(): C(make_shared<connection>("dbname = server_db user = postgres password = postgres")){
    if (C->is_open()){
        cout << "Server successfully connect to the databaase." <<endl;
        this->DropAllTables();
        this->createTables();
    }
    else{
        throw "Server unable to connect to the database. Server exit...";
    }
}

ServerData::~ServerData(){
    C->disconnect();
}

void ServerData::createTables(){
    string table_file = 
    "CREATE TABLE IF NOT EXISTS FILE("\
    "FILENAME VARCHAR(100) NOT NULL,"\
    "FILESIZE INT NOT NULL,"\
    "TTL INT NOT NULL,"\
    "IP INT NOT NULL,"\
    "PORT INT NOT NULL,"\
    "PRIMARY KEY (FILENAME));";

    work W(*C);
    W.exec(table_file);
    W.commit();
}

void ServerData::DropAllTables(){
    work W(*C);
    string drop_file = "DROP TABLE IF EXISTS FILE CASCADE;";
    W.exec(drop_file);
    W.commit();
}

result ServerData::is_file(work &W, string file_name){
    string is_file = "SELECT * FROM FILE WHERE FILENAME = '" + file_name + "' FOR UPDATE;"; 
    result R(W.exec(is_file));
    return R;
}
int ServerData::add_file(string file_name, int file_size, int file_ttl, int user_ip, int port){
    std::lock_guard <std::mutex> lck(mtx);
    work W(*C);
    result R = ServerData::is_file(W,file_name);
    if(R.size()==1){
        return 0;
    }
    else{
        string insert_file = "INSERT INTO FILE VALUES ('" + file_name + "'," + to_string(file_size) + ","
        + to_string(file_ttl) + "," + to_string(user_ip) + "," + to_string(port) + ");";
        W.exec(insert_file);
    }
    W.commit();
    return 1;
}

S2CQuery ServerData::query_file(string file_name){
    std::lock_guard <std::mutex> lck(mtx);
    work W(*C);
    result R = ServerData::is_file(W,file_name);
    if(R.size() == 1){
        int file_size = R[0][1].as<int>();
        int file_ttl = R[0][2].as<int>();
        int ip = R[0][3].as<int>();
        int port = R[0][4].as<int>();
        int new_ttl = file_ttl -1;

        if(new_ttl>0){
            string query_file = "UPDATE FILE SET TTL = " + to_string(new_ttl) + " WHERE FILENAME = '" + file_name + "';";
            W.exec(query_file);
        }
        else{
            string delete_file = "DELETE FROM FILE WHERE FILENAME = '" + file_name + "';";
            W.exec(delete_file);
            if(new_ttl < 0){
                S2CQuery response;
                response.set_resp(false);
                return response; 
            }
        }
        W.commit();
        S2CQuery response;
        response.set_resp(true);
        response.set_file_name(file_name);
        response.set_file_size(file_size);
        response.set_target_ip(ip);
        response.set_target_port(port);
        return response;
    }
    else{
        S2CQuery response;
        response.set_resp(false);
        return response;
    }
}

int ServerData::delete_file(string file_name){
    std::lock_guard <std::mutex> lck(mtx);
    work W(*C);
    try{
        result R = ServerData::is_file(W,file_name);
        if(R.size()==0){
            return 0;
        }
        string delete_file = "DELETE FROM FILE WHERE FILENAME = '" + file_name + "';";
        W.exec(delete_file);
        W.commit();
        return 1;
    }
    catch(const std::exception &e){
        W.abort();
        return 0;
    }
}

int ServerData::delete_user(int user_ip){
    std::lock_guard <std::mutex> lck(mtx);
    work W(*C);
    string delete_user = "DELETE FROM FILE WHERE IP = " + to_string(user_ip) + ";";
    W.exec(delete_user);
    W.commit();
    return 1;
}