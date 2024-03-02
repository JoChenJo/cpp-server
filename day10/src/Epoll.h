#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll
{
public:
    Epoll();
    ~Epoll();

    // void addFD(int fd, uint32_t op);
    void updateChannel(Channel*);

    std::vector<Channel*> poll(int timeout = -1);
private:
    int epfd;
    struct epoll_event *events;
    const int MAX_EVENTS = 1000;
};