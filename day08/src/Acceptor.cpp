#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop)
{
    sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    sock->Bind(addr);
    sock->Listen();
    sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->getFD());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
    delete addr;
}

Acceptor::~Acceptor(){
    delete sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection(){
    InetAddress *cli_addr = new InetAddress();
    Socket *cli_sock = new Socket(sock->Accept(cli_addr));
    printf("new client fd %d! IP: %s Port: %d\n", cli_sock->getFD(), inet_ntoa(cli_addr->addr.sin_addr), ntohs(cli_addr->addr.sin_port));
    cli_sock->setnonblocking();
    newConnectionCallback(cli_sock);
    delete cli_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb){
    newConnectionCallback = _cb;
}