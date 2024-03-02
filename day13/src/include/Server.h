#pragma once
#include <map>
#include <vector>
#include "Macros.h"
class Acceptor;
class EventLoop;
class Socket;
class Connection;
class ThreadPool;
class Server
{
public:
    explicit Server(EventLoop*);
    ~Server();
    DISALLOW_COPY_AND_MOVE(Server);

    void newConnection(Socket *serv_sock);
    void deleteConnection(int sockfd);
private:
    EventLoop *mainReactor;
    Acceptor *acceptor;
    std::map<int , Connection*> connections;
    std::vector<EventLoop*> subReactors;
    ThreadPool *thpool;
};