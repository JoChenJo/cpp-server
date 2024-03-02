#include "Socket.h"
#include <sys/socket.h>
#include "util.h"
#include "InetAddress.h"
#include <fcntl.h>
#include <memory>
#include <unistd.h>

Socket::Socket():fd(-1){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}

Socket::Socket(int _fd):fd(_fd){
    errif(fd == -1, "socket create error");
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Socket::Bind(InetAddress *addr){
    errif(bind(fd, (sockaddr*)&addr->addr, addr->addr_len ) == -1, "socket bind error");
}

void Socket::Listen(){
    errif(listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::Accept(InetAddress * addr){
    //std::shared_ptr<InetAddress> addr = std::make_shared<InetAddress>();
    int cfd = accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(cfd == -1, "socket accept error");
    return cfd;
}

int Socket::getFD(){
    return fd;
}