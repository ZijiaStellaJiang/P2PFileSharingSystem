#include <string>
#include "request.h"

int request::getFd() const {
    return fd;
}

const std::string & request::getIpAddress() const {
    return ip_address;
}