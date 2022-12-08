#include "database.h"

int main(){
    try{
        ServerData db;
        string file_name ="example";
        int file_size = 100;
        int file_ttl = 50;
        int ip = 1233423452;
        int port = 1111;
        int res = db.add_file(file_name,file_size,file_ttl,ip,port);
        cout<<"add 1 result " << res <<endl;

        string file_name2 ="example2";
        int file_size2 = 100;
        int file_ttl2 = 50;
        int ip2 = 1233423452;
        int port2 = 1111;
        int res2 = db.add_file(file_name2,file_size2,file_ttl2,ip2,port2);
        cout<<"add 2 result " << res2 <<endl;

        string file_name3 = "query_Test";
        S2CQuery res_query = db.query_file(file_name3);
        cout<<"query file: " <<file_name2<<" result: "<<endl;
        cout<<"response " <<res_query.resp() <<endl;
        cout<<"file name " << res_query.file_name() << endl;
        cout<<"file size " << res_query.file_size() << endl;
        cout<<"ip " << res_query.target_ip() <<endl;
        cout<<"port " << res_query.target_port() << endl;

        // int delete_res = db.delete_file(file_name2);
        // cout<<"delete result "<<delete_res<<endl;

        int unuse_ip = 1111111111;
        int delete_user = db.delete_user(unuse_ip);
        cout<<"delete user result "<<delete_user<<endl;
    }
    catch(string s){
        cout<<s<<endl;
        return -1;
    }
    return 0;
}