#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>
#include <sys/epoll.h>
#include "Socket.h"

Channel::Channel(EventLoop *_loop, int _fd):loop(_loop), socket(socket), listenevent(0), ready(0), inEpoll(false){

}

Channel::~Channel(){
    loop->deleteChannel(this);
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

Socket *Channel::GetSocket(){
    return socket;
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

void Channel::setReady(uint32_t ev){
    if (ev & READ_EVENT) {
    ready |= READ_EVENT;
  }
  if (ev & WRITE_EVENT) {
    ready |= WRITE_EVENT;
  }
  if (ev & ET) {
    ready |= ET;
  }
}

void Channel::setReadCallback(std::function<void()> _cb){
    readCallback = _cb;
}

void Channel::setWriteCallback(std::function<void()> callback) {
     writeCallback = callback;
}