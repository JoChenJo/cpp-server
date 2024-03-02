#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Acceptor.h"
#include "Connection.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <memory>

Server::Server(EventLoop * _loop): loop(_loop), acceptor(nullptr){
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server(){
    delete acceptor;
}


void Server::newConnection(Socket *sock){
    Connection * conn = new Connection(loop, sock);
    std::function<void(Socket*)> cb = std::bind(&Server::deleteConnection , this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getFD()] = conn;
}

void Server::deleteConnection(Socket *sock){
    Connection *conn = connections[sock->getFD()];
    connections.erase(sock->getFD());
    delete conn;
}