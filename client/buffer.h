#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>
#include <utility>
using namespace std;
class buffer {
public:

    static uint32_t getWholeBufferLength(vector<char> buffer);
    static uint32_t getContentLength(vector<char> buffer);
};

#endif
