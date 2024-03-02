#pragma once
#include <map>
#include <vector>
#include <functional>
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
    void deleteConnection(Socket * sock);
    void OnConnect(std::function<void(Connection *)> fn);
    void OnMessage(std::function<void(Connection *)> fb);
    void NewConnect(std::function<void(Connection *)> fb);
private:
    EventLoop *mainReactor;
    Acceptor *acceptor;
    std::map<int , Connection*> connections;
    std::vector<EventLoop*> subReactors;
    ThreadPool *thpool;
    std::function<void(Connection *)> on_connection_callback;
    std::function<void(Connection *)> on_message_callback;
    std::function<void(Connection *)> new_connect_callback;
};