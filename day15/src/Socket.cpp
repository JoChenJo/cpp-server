#include "Socket.h"
#include <sys/socket.h>
#include "util.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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
    int cli_sockfd = -1;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if(fcntl(fd, F_GETFL) & O_NONBLOCK){
        while(true){
            cli_sockfd = accept(fd, (sockaddr*)&addr, &addr_len);
            if(cli_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
                continue;
            }else if(cli_sockfd == -1){
                errif(true, "socket accept error");
            }else{
                break;
            }
        }
    }
    else{
        cli_sockfd = accept(fd, (sockaddr*)&addr, &addr_len);
        errif(cli_sockfd == -1, "socket accept error");
    }
    _addr->setInetAddr(addr);
    return cli_sockfd;
}

void Socket::Connect(InetAddress* _addr){
    struct sockaddr_in addr = _addr->getAddr();
    if(fcntl(fd, F_GETFL) & O_NONBLOCK){
        while(true){
            int ret = connect(fd, (sockaddr*)&addr, sizeof(addr));
            if(ret == 0) break;
            else if(ret == -1 && EINPROGRESS) continue;
            else if(ret == -1) errif(true, "socket connect error");
        }
    }else errif(connect(fd, (sockaddr*)&addr, sizeof(addr)) == -1, "socket connect error");
}

void Socket::Connect(const char *ip, uint16_t port) {
  InetAddress *addr = new InetAddress(ip, port);
  Connect(addr);
  delete addr;
}

int Socket::getFD(){
    return fd;
}
//===========================================================================================================================
//InetAddress
InetAddress::InetAddress() = default;

InetAddress::InetAddress(const char* ip, uint16_t port){
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
}

InetAddress::~InetAddress(){
}

sockaddr_in InetAddress::getAddr(){
    return addr;
}

void InetAddress::setInetAddr(sockaddr_in _addr){
    addr = _addr;
}

char* InetAddress::getIp(){
    return inet_ntoa(addr.sin_addr);
}

uint16_t InetAddress::getPort(){
    return ntohs(addr.sin_port);
}