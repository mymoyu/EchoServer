#ifndef EPOLL_H__
#define EPOLL_H__

#include <vector>
struct epoll_event;

class Channel;

class Epoll
{
private:
    #define MAX_EVENTS 1024
    // epoll 文件描述符
    int epfd;
    epoll_event* events;
public:
    Epoll();
    ~Epoll();

    void addclnt(int fd, int op);
    std::vector<Channel*> poll_wait(int timeout = -1);
    void updateChannel(Channel*);
    void deleteChannel(Channel *);
};

#endif

/*
    epoll_event结构体里, 可以利用空指针ptr来指向某个特定的事件
*/