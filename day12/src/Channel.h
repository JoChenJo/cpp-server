#pragma once
#include <functional>

class EventLoop;
class Channel
{
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableRead();

    int getFD();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool _in = true);

    void setReady(uint32_t);

    void useET();
    void setReadCallback(std::function<void()>);
private:
    EventLoop *loop;
    int fd;
    uint32_t events;    
    uint32_t ready;   
    bool inEpoll;       

    std::function<void()> readCallback;
    std::function<void()> writeCallback;
};