#include <vector>
#include "./include/EventLoop.h"
#include "./include/Channel.h"
#include "./include/Epoll.h"
#include "./include/ThreadPool.h"

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
    threadPool = new ThreadPool();
}

EventLoop::~EventLoop() {
    delete threadPool;
    delete ep;
}

void EventLoop::loop() {
    while (!quit)
    {
        std::vector<Channel*> chs;
        chs = ep->poll_wait();
        for (auto it = chs.begin(); it != chs.end(); ++it)
        {
            // 执行事件
            (*it)->handleEvent();
        }
        
    }
    
}
void EventLoop::updateChannel(Channel* ch) {
    ep->updateChannel(ch);
}

// 将事件加入任务队列
void EventLoop::addThread(std::function<void()> func) {
    threadPool->add(func);
}