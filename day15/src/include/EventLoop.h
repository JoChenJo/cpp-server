#pragma once
#include <functional>
#include "Macros.h"
class Epoll;
class Channel;
class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    DISALLOW_COPY_AND_MOVE(EventLoop);

    void loop();
    void updateChannel(Channel*);
    void deleteChannel(Channel *ch);
    void Quit();
private:
    Epoll *ep;
    bool quit;
};