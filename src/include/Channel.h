#ifndef CHANNEL_H__
#define CHANNEL_H__

#include <stdint.h>
#include <functional>

class EventLoop;

class Channel
{
private:
    EventLoop* loop;
    int fd; /* 监控的fd */
    uint32_t events; /* 表示希望监听这个文件描述符的哪些事件 */
    // uint32_t revents; /* 表示在epoll返回该Channel时文件描述符正在发生的事件, 即负责的事件 */
    uint32_t ready;
    bool inEpoll;
    bool useThreadPool;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
public:
    Channel() = delete;
    Channel(EventLoop*, int);
    // Channel(EventLoop *, int);
    ~Channel();

    void handleEvent();
    // 设置channel填入epoll中
    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool flag = true);
    void useET();

    // void setEvent(uint32_t);
    void setReady(uint32_t);
    void setReadCallBack(std::function<void()>);
    void setUseThreadPool(bool use = true);
};

#endif