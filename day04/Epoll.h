#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll
{
public:
    Epoll();
    ~Epoll();

    void addFD(int fd, uint32_t op);
    std::vector<epoll_event> poll(int timeout = -1);
private:
    int epfd;
    struct epoll_event *events;
    const int MAX_EVENTS = 1000;
};