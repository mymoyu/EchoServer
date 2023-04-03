#include <memory>
#include <functional>
class EventLoop;
class Socket;
class InetAddress;
class Channel;

// 承担服务器的创建socket工作和调度工作
class Acceptor
{
private:
    // EventLoop *loop;
    EventLoop* loop;
    Socket *sock;
    Channel *acceptChannel;
    std::function<void(Socket*)> newConnectionCallback;
public:
    Acceptor(EventLoop*, const char *);
    ~Acceptor();
    // 接受连接
    void acceptConnection();
    // 建立连接并绑定事件
    void setNewConnectionCallback(std::function<void(Socket*)>);
};