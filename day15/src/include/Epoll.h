#pragma once
#include <vector>
#include "Macros.h"
#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

class Channel;
class Epoll
{
public:
    Epoll();
    ~Epoll();
    DISALLOW_COPY_AND_MOVE(Epoll);

    // void addFD(int fd, uint32_t op);
    void updateChannel(Channel*);
    void deleteChannel(Channel*);
    std::vector<Channel*> poll(int timeout = -1);
private:
    int epfd;
    #ifdef OS_LINUX
        struct epoll_event *events;
    #endif

    #ifdef OS_MACOS
        struct kevent *events_{nullptr};
    #endif

    const int MAX_EVENTS = 1000;
};