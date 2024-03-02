#pragma once

class InetAddress;
class Socket
{
public:
    Socket();
    Socket(int);
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