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

int ServerData::add_file(int user_id, int user_ip, string file_name, int file_size, int file_ttl, int port){
    work W(*C);
    return 1;
}

S2CQuery ServerData::query_file(string file_name){
    
}
int ServerData::delete_file(string file_name){

}
int ServerData::delete_user(int user_id){

}