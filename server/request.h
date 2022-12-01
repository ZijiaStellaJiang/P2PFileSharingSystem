#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>
using namespace std;

class request{
private:
    int fd; // record the client fd
    std::string ip_address; // record the client ip_address;

public:
    request(int fd, std::string ip_address): fd(fd), ip_address(ip_address){};

    int getFd() const;

    const std::string &getIpAddress() const;
};

#endif