#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "Socket.h"
#include "InetAddress.h"
#include <memory>

int main(){
    const int BUFFER_SIZE = 1024;
    std::shared_ptr<Socket> cfd_ptr = std::make_shared<Socket>();

    std::shared_ptr<InetAddress> addr_ptr = std::make_shared<InetAddress>("127.0.0.1", 8888);
    
    errif(connect(cfd_ptr->getFD(), (sockaddr*)&addr_ptr->addr, addr_ptr->addr_len) == -1, "socket connect error");

    while (true)
    {
        char buf[BUFFER_SIZE];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t bytes_write = write(cfd_ptr->getFD(), buf, sizeof(buf));
        if(bytes_write == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(cfd_ptr->getFD(), buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from server: %s\n", buf);
        }else if(bytes_read == 0){
            printf("server socket disconnect");
            break;
        }else if(bytes_read == -1){
            close(cfd_ptr->getFD());
            errif(true, "socket read error");
        }
    }
    close(cfd_ptr->getFD());
    return 0;
}