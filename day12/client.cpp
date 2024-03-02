#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "src/util.h"
#include "src/Socket.h"
#include "src/InetAddress.h"
#include "src/Buffer.h"
#include <memory>

int main(){
    std::shared_ptr<Socket> cfd_ptr = std::make_shared<Socket>();
    //std::shared_ptr<InetAddress> addr_ptr = std::make_shared<InetAddress>("127.0.0.1", 8888);
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    cfd_ptr->Connect(addr);

    int sockfd = cfd_ptr->getFD();
    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();
    
    while (true)
    {
        sendBuffer->getline();
        ssize_t bytes_write = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
        if(bytes_write == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        int already_read = 0;
        char buf[1024];
        while (true)
        {
            bzero(&buf, sizeof(buf));
            ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
            if(bytes_read > 0){
                readBuffer->append(buf, bytes_read);
                already_read += bytes_read;
            }else if(bytes_read == 0){
                printf("server socket disconnect\n");
                exit(EXIT_SUCCESS);
            }
            if(already_read >= sendBuffer->size()){
                printf("message from server: %s\n", readBuffer->c_str());
                break;
            }
        }
        readBuffer->clear();
    }
    delete addr;
    
    return 0;
}