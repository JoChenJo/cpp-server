#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(int args, char* argv[])
{

    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(8888);
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(lfd, (sockaddr*)&ser_addr, sizeof(ser_addr));

    listen(lfd, SOMAXCONN);

    struct sockaddr_in cli_addr;
    bzero(&cli_addr, sizeof(cli_addr));
    socklen_t cli_addr_len = sizeof(cli_addr);

    int cfd = accept(lfd, (sockaddr *)&cli_addr, &cli_addr_len);
    printf("new client fd %d! IP: %s Port: %d\n", cfd, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

    return 0;
}