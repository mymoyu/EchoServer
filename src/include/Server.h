#ifndef SERVER_H__
#define SERVER_H__

#include <map>
#include <vector>
class EventLoop;
class Acceptor;
class Socket;
class Channel;
class Connection;
class ThreadPool;
class Server
{
private:
    EventLoop *mainReactor; // 负责连接的处理
    Acceptor *acceptor; // 连接接收者
    // 存放连接的map
    std::map<int, Connection*> connections; // TCP连接
    std::vector<EventLoop*> subReactors; // 负责处理事件循环
    ThreadPool *thpool; // 线程池
public:
    Server() = delete;
    Server(EventLoop*,  const char *);
    ~Server();

    // 读取信息事件
    void handleReadEvent(Channel*);
    void newConnection(Socket* );
    // 删除无用的连接资源
    void deleteConnection(int);
};

#endif