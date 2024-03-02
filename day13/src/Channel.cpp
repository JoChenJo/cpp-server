#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>
#include <sys/epoll.h>

Channel::Channel(EventLoop *_loop, int _fd):loop(_loop), fd(_fd), listenevent(0), ready(0), inEpoll(false){

}

Channel::~Channel(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Channel::handleEvent(){
    //callback();
    //loop->addThread(callback);
    if(ready & (EPOLLIN | EPOLLPRI)){
        readCallback();
    }
    if(ready & EPOLLOUT){
        writeCallback();
    }
}

void Channel::enableRead(){
    listenevent |= EPOLLIN | EPOLLPRI;
    loop->updateChannel(this);
}

void Channel::useET(){
    listenevent |= EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFD(){
    return fd;
}

uint32_t Channel::getListen(){
    return listenevent;
}

uint32_t Channel::getReady(){
    return ready;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(bool _in){
    inEpoll = _in;
}

void Channel::setReady(uint32_t _ev){
    ready = _ev;
}

void Channel::setReadCallback(std::function<void()> _cb){
    readCallback = _cb;
}