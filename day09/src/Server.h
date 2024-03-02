#pragma once
#include <map>
class Acceptor;
class EventLoop;
class Socket;
class Connection;
class Server
{
public:
    Server(EventLoop*);
    ~Server();

    void handleRreadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *);
private:
    EventLoop *loop;
    const int READ_BUFFER = 1024;
    Acceptor *acceptor;
    std::map<int , Connection*> connections;
};