#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(int args, char* argv[])
{
    struct sockaddr_in cli_addr;
    bzero(&cli_addr, sizeof(cli_addr));

    int cfd = socket(AF_INET, SOCK_STREAM, 0);

    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(8888);
    cli_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(cfd, (sockaddr *)&cli_addr, sizeof(cli_addr));



    return 0;
}