#pragma once
#include <functional>
#include "Macros.h"
class Socket;
class EventLoop;
class Channel
{
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();
    DISALLOW_COPY_AND_MOVE(Channel);

    void handleEvent();
    void enableRead();
    void enableWrite();
    
    Socket *GetSocket();
    int getFD();
    uint32_t getListen();
    uint32_t getReady();

    bool getInEpoll();
    void setInEpoll(bool _in = true);
    void useET();

    void setReady(uint32_t);
    void setReadCallback(std::function<void()>);
    void setWriteCallback(std::function<void()>);

    static const int READ_EVENT;    //NOLINT
    static const int WRITE_EVENT;   //NOLINT
    static const int ET;            //NOLINT
private:
    EventLoop *loop;
    Socket *socket;
    uint32_t listenevent;    
    uint32_t ready;   
    bool inEpoll;       

    std::function<void()> readCallback;
    std::function<void()> writeCallback;
};