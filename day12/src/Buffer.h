#pragma once
#include <string>

class Buffer
{
public:
    Buffer();
    ~Buffer();
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