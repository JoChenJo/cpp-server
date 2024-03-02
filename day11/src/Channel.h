#pragma once
#include <sys/epoll.h>
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
    void setUseThreadPool(bool use = true);
private:
    EventLoop *loop;
    int fd;
    uint32_t events;    //表示希望监听这个文件描述符的哪些事件
    uint32_t ready;   
    bool inEpoll;       //表示当前`Channel`是否已经在`epoll`红黑树中
    bool useThreadPool;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
};