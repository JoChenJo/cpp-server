#pragma once
#include <map>
#include <vector>
class Acceptor;
class EventLoop;
class Socket;
class Connection;
class ThreadPool;
class Server
{
public:
    Server(EventLoop*);
    ~Server();

    void handleRreadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(int sockfd);
private:
    EventLoop *mainReactor;
    Acceptor *acceptor;
    std::map<int , Connection*> connections;
    std::vector<EventLoop*> subReactors;
    ThreadPool *thpool;
};