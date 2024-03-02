#pragma once
#include <arpa/inet.h>
class InetAddress

{
public:
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();

    void setInetAddr(sockaddr_in _addr);
    sockaddr_in getAddr();
    char* getIp();
    uint16_t getPort();
private:
    struct sockaddr_in addr;
};

class Socket
{
public:
    Socket();
    Socket(int _fd);
    ~Socket();

    void Bind(InetAddress*);
    void Listen();
    int Accept(InetAddress*);
    void Connect(InetAddress*);
    
    void setnonblocking();
    int getFD();
private:
    int fd;
};