#pragma once

class Acceptor;
class EventLoop;
class Socket;
class Server
{
public:
    Server(EventLoop*);
    ~Server();

    void handleRreadEvent(int);
    void newConnection(Socket *serv_sock);
private:
    EventLoop *loop;
    const int READ_BUFFER = 1024;
    Acceptor *acceptor;
};