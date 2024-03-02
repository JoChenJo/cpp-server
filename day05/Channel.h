#pragma once
#include <sys/epoll.h>

class Epoll;
class Channel
{
public:
    Channel(Epoll *_ep, int _fd);
    ~Channel();

    void enableReading();

    int getFD();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    void setRevents(uint32_t);

private:
    Epoll *ep;
    int fd;
    uint32_t events;    //表示希望监听这个文件描述符的哪些事件
    uint32_t revents;   //表示在`epoll`返回该`Channel`时文件描述符正在发生的事件
    bool inEpoll;       //表示当前`Channel`是否已经在`epoll`红黑树中
};