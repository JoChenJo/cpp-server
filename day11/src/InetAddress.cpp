#include "InetAddress.h"
#include <string.h>

InetAddress::InetAddress(){
    bzero(&addr, sizeof(addr));
}

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