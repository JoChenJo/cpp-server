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
    struct epoll_event *events;
    const int MAX_EVENTS = 1000;
};