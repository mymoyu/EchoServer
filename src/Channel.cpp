#include <sys/epoll.h>
#include <unistd.h>
#include "./include/Epoll.h"
#include "./include/Channel.h"
#include "./include/EventLoop.h"

Channel::Channel(EventLoop* _loop, int _fd) : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false), useThreadPool(false) {}
// Channel::Channel(EventLoop *_loop, int _fd) : loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {
    if(fd != -1) {
        close(fd);
        fd = -1;
    }
}

// 将可读或可写事件写入任务队列中
void Channel::handleEvent() {
    if(ready & (EPOLLIN | EPOLLPRI)){
        if(useThreadPool)       
            loop->addThread(readCallback);
        else
            readCallback();
    }
    if(ready & (EPOLLOUT)){
        if(useThreadPool)       
            loop->addThread(writeCallback);
        else
            writeCallback();
    }
}

void Channel::enableReading() {
    // EPOLLET
    events |= EPOLLIN | EPOLLPRI;
    loop->updateChannel(this);
}

void Channel::useET() {
    events |= EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd() {
    return fd;
}

uint32_t Channel::getEvents() {
    return events;
}

uint32_t Channel::getReady() {
    return ready;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(bool flag){
    inEpoll = flag;
}

// void Channel::setEvent(uint32_t) {

// }
void Channel::setReady(uint32_t _ev) {
    ready = _ev;
}

void Channel::setReadCallBack(std::function<void()> _cb) {
    readCallback = _cb;
}

void Channel::setUseThreadPool(bool use){
    useThreadPool = use;
}