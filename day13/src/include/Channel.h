#pragma once
#include <functional>
#include "Macros.h"

class EventLoop;
class Channel
{
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableRead();
    DISALLOW_COPY_AND_MOVE(Channel);

    int getFD();
    uint32_t getListen();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool _in = true);

    void setReady(uint32_t);

    void useET();
    void setReadCallback(std::function<void()>);
private:
    EventLoop *loop;
    int fd;
    uint32_t listenevent;    
    uint32_t ready;   
    bool inEpoll;       

    std::function<void()> readCallback;
    std::function<void()> writeCallback;
};