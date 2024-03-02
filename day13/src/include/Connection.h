#pragma once
#include <functional>
#include "Macros.h"
class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection
{
public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();
    DISALLOW_COPY_AND_MOVE(Connection);

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(int)> const &callback);
    void send(int sockfd);
private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    std::function<void(int)> deleteConnectionCallback;
    Buffer *readBuffer;
};