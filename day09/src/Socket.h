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
    void setnonblocking();

    int Accept(InetAddress*);
    void Connect(InetAddress*);

    int getFD();
private:
    int fd;
};