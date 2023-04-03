#ifndef EVENTLOOP_H__
#define EVENTLOOP_H__

#include <functional>
class Epoll;
class Channel;
class ThreadPool;
class EventLoop
{
private:
    Epoll *ep;
    ThreadPool *threadPool;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel*);

    void addThread(std::function<void()>);
};

#endif