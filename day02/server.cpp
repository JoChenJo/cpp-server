#include <stdio.h>
#include "util.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int main(int args, char* argv[]) {
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(lfd == -1, "socket create error");

    struct sockaddr_in ser_addr, cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);

    bzero(&ser_addr, sizeof(ser_addr));
    bzero(&cli_addr, cli_addr_len);

    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(8888);
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    errif(bind(lfd, (sockaddr*)&ser_addr, sizeof(ser_addr)) == -1, "socket bind error");

    errif(listen(lfd, SOMAXCONN) == -1, "socket listen error");

    int cfd = accept(lfd, (sockaddr*)&cli_addr, &cli_addr_len);

    errif(cfd == -1, "socket accept error");


    while(true){
        char buf[1024];
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(cfd, &buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from client fd %d: %s\n", cfd, buf);
            for(int i = 0;i < read_bytes;++i)
            {
                buf[i] = toupper(buf[i]);
            }
            write(cfd, &buf, sizeof(buf));
        }else if(read_bytes == 0){
            printf("client fd %d disconnected\n", cfd);
            close(cfd);
            break;
        }else if(read_bytes == -1){
            close(cfd);
            errif(true, "socket read error");
        }
    }
    close(lfd);

    return 0;
}