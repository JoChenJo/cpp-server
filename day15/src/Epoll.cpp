#include "Epoll.h"
#include "util.h"
#include <unistd.h>
#include <string.h>
#include "Channel.h"

#ifdef OS_LINUX

Epoll::Epoll():epfd(1), events(nullptr){
    epfd = epoll_create1(0);
    errif(epfd == -1, "socket epoll_createl error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}


std::vector<Channel*> Epoll::poll(int timeout){
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for(int i = 0; i < nfds; ++i){
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setReady(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

void Epoll::updateChannel(Channel* channel){
    int fd = channel->getFD();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getListen();
    if(!channel->getInEpoll()){
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "socket epoll_ctl_add error");
        channel->setInEpoll();
    }else{
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "socket epoll_ctl_mod error");
    }
}

void Epoll::deleteChannel(Channel* channel){
    int fd = channel->getFD();
    errif(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1, "epoll delete error");
    channel->setInEpoll(false);
}

#endif

#ifdef OS_MACOS

Epoll::Epoll():epfd(1), events(nullptr){
    epfd = kqueue();
    errif(epfd == -1, "socket epoll_createl error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}


std::vector<Channel*> Epoll::poll(int timeout){
    std::vector<Channel*> activeChannels;
    struct timespec ts;
    memset(&ts, 0, sizeof(ts));
    if (timeout != -1) {
        ts.tv_sec = timeout / 1000;
        ts.tv_nsec = (timeout % 1000) * 1000 * 1000;
    }
    int nfds = 0;
    if (timeout == -1) {
        nfds = kevent(fd_, NULL, 0, events_, MAX_EVENTS, NULL);
    } else {
        nfds = kevent(fd_, NULL, 0, events_, MAX_EVENTS, &ts);
    }
    for (int i = 0; i < nfds; ++i) {
        Channel *ch = (Channel *)events_[i].udata;
        int events = events_[i].filter;
    if (events == EVFILT_READ) {
      ch->SetReadyEvents(ch->READ_EVENT | ch->ET);
    }
    if (events == EVFILT_WRITE) {
      ch->SetReadyEvents(ch->WRITE_EVENT | ch->ET);
    }
        active_channels.push_back(ch);
    }
  return active_channels;
}

void Epoll::updateChannel(Channel* channel){
  struct kevent ev[2];
  memset(ev, 0, sizeof(*ev) * 2);
  int n = 0;
  int fd = ch->GetSocket()->GetFd();
  int op = EV_ADD;
  if (ch->GetListenEvents() & ch->ET) {
    op |= EV_CLEAR;
  }
  if (ch->GetListenEvents() & ch->READ_EVENT) {
    EV_SET(&ev[n++], fd, EVFILT_READ, op, 0, 0, ch);
  }
  if (ch->GetListenEvents() & ch->WRITE_EVENT) {
    EV_SET(&ev[n++], fd, EVFILT_WRITE, op, 0, 0, ch);
  }
  int r = kevent(fd_, ev, n, NULL, 0, NULL);
  ErrorIf(r == -1, "kqueue add event error");
}

void Epoll::deleteChannel(Channel* channel){
    struct kevent ev[2];
    int n = 0;
    int fd = ch->GetSocket()->GetFd();
    if (ch->GetListenEvents() & ch->READ_EVENT) {
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, ch);
    }
    if (ch->GetListenEvents() & ch->WRITE_EVENT) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, ch);
    }
    int r = kevent(fd_, ev, n, NULL, 0, NULL);
    ErrorIf(r == -1, "kqueue delete event error");
}

#endif