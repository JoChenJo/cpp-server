#pragma once
#include <arpa/inet.h>
#include "Macros.h"
class InetAddress
{
public:
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
    DISALLOW_COPY_AND_MOVE(InetAddress);

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
    explicit Socket(int _fd);
    ~Socket();
    DISALLOW_COPY_AND_MOVE(Socket);

    void Bind(InetAddress*);
    void Listen();
    int Accept(InetAddress*);
    void Connect(InetAddress*);
     void Connect(const char *ip, uint16_t port);
    
    void setnonblocking();
    bool IsNonBlocking();
    int getFD();
private:
    int fd;
};