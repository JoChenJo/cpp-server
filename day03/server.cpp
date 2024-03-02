#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include "util.h"

int main(){
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(lfd == -1, "socket create error");

    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(8888);
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //ser_addr.sin_addr.s_addr = INADDR_ANY;

    errif(bind(lfd, (sockaddr*)&ser_addr, sizeof(ser_addr)) == -1, "socket bind error");
    errif(listen(lfd, 128) == -1, "socket listen error");

    int epfd = epoll_create1(0);
    errif(epfd == -1, "socket epoll_create error");
    struct epoll_event events[1024], ev;
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));

    ev.data.fd = lfd;
    ev.events = EPOLLIN;
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev) == -1, "socket epoll_ctl_add error");

    while(true){
        
        int nfds = epoll_wait(epfd, events, 1024, -1);
        printf("%d\n", nfds);
        errif(nfds == -1, "socket epoll_wait error");
        for(int i = 0; i < nfds; ++i){
            printf("for\n");
            if(events[i].data.fd == lfd){
                struct sockaddr_in cli_addr ;
                socklen_t cli_addr_len = sizeof(cli_addr);
                bzero(&cli_addr, cli_addr_len);
                int cfd = accept(lfd, (sockaddr*)&cli_addr, &cli_addr_len);
                errif(cfd == -1, "socket accept error");
                printf("new client fd: %d! IP: %s  Port: %d\n", cfd, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            
                bzero(&ev, sizeof(ev));
                ev.data.fd = cfd;
                ev.events = EPOLLIN | EPOLLET;
                fcntl(cfd, F_SETFL, fcntl(cfd, F_GETFL) | O_NONBLOCK);
                errif(epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev) == -1, "socket epoll_ctl_add error");
            }else if(events[i].events & EPOLLIN){
                
                char buf[1024];
                while(true){
                    bzero(&buf, sizeof(buf));
                    ssize_t read_bytes = read(events[i].data.fd, buf, sizeof(buf));
                    if(read_bytes > 0){
                        printf("message from client fd%d: %s\n", events[i].data.fd, buf);
                        for(int i = 0; i < read_bytes; ++i){
                            buf[i] = toupper(buf[i]);
                        }
                        write(events[i].data.fd, buf, sizeof(buf));
                    }else if(read_bytes == 0){
                        printf("EOF, client fd%d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);
                        break;
                    }else if(events[i].data.fd == -1 && EINTR){
                        printf("continue reading");
                        continue;
                    }else if(events[i].data.fd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
                        printf("finish reading once, errno: %d\n", events[i].data.fd);
                        break;
                    }
                }
            }else {
                printf("something else happened\n");
            }
        }
    }
    close(lfd);
    close(epfd);
    return 0;
}