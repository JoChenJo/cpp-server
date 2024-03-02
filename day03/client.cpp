#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


int main(){
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(cfd == -1, "socket create error");
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    bzero(&cli_addr, sizeof(cli_addr));

    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(8888);
    cli_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //errif(bind(cfd, (sockaddr*)&cli_addr, cli_addr_len) == -1, "socket bind error");

    errif(connect(cfd, (sockaddr*)&cli_addr, cli_addr_len) == -1, "socket connect error");

    while (true)
    {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(cfd, buf, sizeof(buf));
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(cfd, buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from server: %s\n", buf);
        }else if(read_bytes == 0){
            printf("server socket disconnect!");
            break;;
        }else if(read_bytes == -1){
            close(cfd);
            errif(true, "socket read error");
        }

    }
    close(cfd);
    return 0;
}