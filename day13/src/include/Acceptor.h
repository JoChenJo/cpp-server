#pragma once
#include <functional>
#include "Macros.h"

class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor
{
public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    DISALLOW_COPY_AND_MOVE(Acceptor);
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)>);
private:
    EventLoop *loop;
    Socket *sock;
    Channel *acceptChannel;
    std::function<void(Socket*)> newConnectionCallback;
};