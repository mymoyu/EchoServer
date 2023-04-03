#include <sys/epoll.h>
#include <strings.h>
#include <unistd.h>
#include <iostream>
#include "./include/Channel.h"
#include "./include/Epoll.h"
#include "./include/util.h"

Epoll::Epoll() : epfd(-1), events(nullptr) {
    epfd =  epoll_create1(0);
    errif(epfd < 0, "epoll_create()");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events)*MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd >= 0)
    {
        close(epfd);
        epfd = -1;
    }
    delete []events;
}

void Epoll::addclnt(int fd, int op) {
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) < 0, "epoll_ctl()");
}

std::vector<Channel*> Epoll::poll_wait(int timeout) {
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds < 0, "epoll_wait()");
    for(int i = 0; i < nfds; ++i) {
        Channel* ch = (Channel*) events[i].data.ptr;
        ch->setReady(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

void Epoll::updateChannel(Channel* channel) {
    int fd = channel->getFd();
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel; // 此处由于需要一个特殊的channel事件, 所以该块空间不能被释放
    ev.events = channel->getEvents();
    if (!channel->getInEpoll()) {
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) < 0, "epoll_ctl()");
        channel->setInEpoll(true);
    } else {
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) < 0, "epoll_ctl()");
    }
}

void Epoll::deleteChannel(Channel *channel) {
    int fd = channel->getFd();
    errif(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) < 0, "epoll_ctl() is error");
    channel->setInEpoll(false);
}