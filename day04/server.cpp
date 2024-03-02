#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include <ctype.h>
#include <memory>

void handleReadEvent(int);
int main(){
    const int MAX_EVENTS = 1024;
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->Bind(serv_addr);
    serv_sock->Listen();
    Epoll *ep = new Epoll();
    serv_sock->setnonblocking();

    ep->addFD(serv_sock->getFD(), EPOLLIN | EPOLLET);
    while (true){
        std::vector<epoll_event> events = ep->poll();
        int nfds = events.size();
        for(int i = 0; i < nfds; ++i){
            if(events[i].data.fd == serv_sock->getFD()){
                //std::shared_ptr<InetAddress> cli_addr_ptr(new InetAddress);
                InetAddress *cli_addr = new InetAddress();
                std::shared_ptr<InetAddress> cli_addr_ptr = std::make_shared<InetAddress>(cli_addr);
                //Socket * cli_sock = new Socket(serv_sock->Accept(cli_addr));
                std::shared_ptr<Socket> cli_sock_ptr = std::make_shared<Socket>(serv_sock->Accept(cli_addr));
                printf("new client fd%d ! IP: %s  Port: %d\n", cli_sock_ptr->getFD(), inet_ntoa(cli_addr->addr.sin_addr), ntohs(cli_addr->addr.sin_port));
                cli_sock_ptr->setnonblocking();
                ep->addFD(cli_sock_ptr->getFD(), EPOLLIN | EPOLLET);
            }else if(events[i].events & EPOLLIN){
                handleReadEvent(events[i].data.fd);
            }else {
                printf("something else happened!\n");
            }
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}

void handleReadEvent(int fd){
    const int READ_BUFFER = 1024;
    char buf[READ_BUFFER];
    while(true){
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(fd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", fd, buf);
            for(int i = 0; i < bytes_read; ++i){
                 buf[i] = toupper(buf[i]);
            }
            write(fd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } 
        else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } 
        else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}