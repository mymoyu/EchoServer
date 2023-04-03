#ifndef CONNECTION_H__
#define CONNECTION_H__
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection
{
private:
    EventLoop* loop;
    Socket *sock;
    Channel *ConChannel;
    // 存储清除的方案
    std::function<void(int)> deleteConnectionCallback;
    Buffer *readBuf;

    #define READ_BUFSIZE 1024
public:
    // 创建连接(客户端)
    Connection(EventLoop*, Socket *);
    ~Connection();

    void echo();
    void setDeleteConnectionCallback(std::function<void(int)>);

    Socket* getSock() const { return sock; };
};

#endif