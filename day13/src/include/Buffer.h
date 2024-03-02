#pragma once
#include <string>
#include "Macros.h"

class Buffer
{
public:
    Buffer();
    ~Buffer();
    DISALLOW_COPY_AND_MOVE(Buffer);
    
    void append(const char* _str, int _size);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
    void Toupper();
    void setBuf(const char*);
private:
    std::string buf;
};