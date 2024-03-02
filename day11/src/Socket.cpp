#include "Socket.h"
#include <sys/socket.h>
#include "util.h"
#include "InetAddress.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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

void Socket::Bind(InetAddress *_addr){
    struct sockaddr_in addr = _addr->getAddr();
    errif(bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1, "socket bind error");
}

void Socket::Listen(){
    errif(listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::Accept(InetAddress* _addr){
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    bzero(&addr, addr_len);
    int cfd = accept(fd, (sockaddr*)&addr, &addr_len);
    errif(cfd == -1, "socket accept error");
    _addr->setInetAddr(addr);
    return cfd;
}

void Socket::Connect(InetAddress* _addr){
    struct sockaddr_in addr = _addr->getAddr();
    errif(connect(fd, (sockaddr*)&addr, sizeof(addr)) == -1, "socket connect eror");
}

int Socket::getFD(){
    return fd;
}
