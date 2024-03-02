#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "ThreadPool.h"
#include "EventLoop.h"
#include "util.h"
#include <functional>
#include "Exception.h"

Server::Server(EventLoop * _loop): mainReactor(_loop), acceptor(nullptr), thpool(nullptr){
    if(mainReactor == nullptr){
        throw Exception(ExceptionType::INVALID, "main reactor can't be nullptr!");
    }
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    int size = std::thread::hardware_concurrency();
    thpool = new ThreadPool(size);
    for(int i = 0; i< size; ++i){
        subReactors.push_back(new EventLoop());
    }

    for(int i = 0; i < size; ++i){
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
        thpool->add(std::move(sub_loop));
    }
}

Server::~Server(){
    for(EventLoop *each : subReactors){
        delete each;
    }
    delete acceptor;
    delete thpool;
}


void Server::newConnection(Socket *sock){
    if(sock->getFD() == -1){
    throw Exception(ExceptionType::INVALID_SOCKET, "New Connection error, invalid client socket!");
    }
    
    int random = sock->getFD() % subReactors.size();
    Connection * conn = new Connection(subReactors[random], sock);
    std::function<void(Socket *)> cb = std::bind(&Server::deleteConnection , this, std::placeholders::_1);
    conn->SetDeleteConnectionCallback(cb);
    conn->SetOnMessageCallback(on_message_callback);
    connections[sock->getFD()] = conn;
    if(new_connect_callback){
        conn->SetOnConnectCallback(on_connection_callback);
    }
}

void Server::deleteConnection(Socket * sock){
    int sockfd = sock->getFD();
    auto it = connections.find(sockfd);
    if(it != connections.end())
    {
        Connection *conn = connections[sockfd];
        connections.erase(sockfd);
        delete conn;
        conn = nullptr;
    }
}

void Server::OnConnect(std::function<void(Connection *)> fn) { on_connection_callback = std::move(fn); }

void Server::OnMessage(std::function<void(Connection *)> fn) { on_message_callback = std::move(fn); }

void Server::NewConnect(std::function<void(Connection *)> fn) { new_connect_callback = std::move(fn); }