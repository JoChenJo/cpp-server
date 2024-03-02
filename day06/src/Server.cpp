#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <memory>

Server::Server(EventLoop * _loop): loop(_loop){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->Bind(serv_addr);
    serv_sock->Listen();

    Channel *servChannel = new Channel(loop, serv_sock->getFD());
    std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
    servChannel->setCallback(cb);
    servChannel->enableReading();
}

Server::~Server(){
    
}

void Server::handleRreadEvent(int fd){
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
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

void Server::newConnection(Socket *serv_sock){
    InetAddress *cli_addr = new InetAddress();
    //std::shared_ptr<InetAddress> cli_addr_ptr(cli_addr);
    //Socket *cli_sock = new Socket(serv_sock->Accept(cli_addr));
    //std::shared_ptr<Socket> cli_sock_ptr = std::make_shared<Socket>(serv_sock->Accept(cli_addr));
    Socket *cli_sock = new Socket(serv_sock->Accept(cli_addr));
    printf("new client fd %d! IP: %s Port: %d\n", cli_sock->getFD(), inet_ntoa(cli_addr->addr.sin_addr), ntohs(cli_addr->addr.sin_port));
    cli_sock->setnonblocking();
    //std::shared_ptr<Channel> cliChannel_ptr = std::make_shared<Channel>(loop, cli_sock->getFD());
    Channel *cliChannel = new Channel(loop, cli_sock->getFD());
    //std::shared_ptr<Channel> cliChannel_ptr(cliChannel);
    std::function<void()> cb = std::bind(&Server::handleRreadEvent, this, cli_sock->getFD());
    cliChannel->setCallback(cb);
    cliChannel->enableReading();
}