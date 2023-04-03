#include "./include/Acceptor.h"
#include "./include/Socket.h"
#include "./include/Channel.h"
#include "./include/InetAddress.h"
#include "./include/EventLoop.h"

Acceptor::Acceptor(EventLoop* _loop, const char *port): loop(_loop) {
    // 由于服务器的连接是长时间的, 故应该由析构函数释放这块空间
    sock = new Socket();
    InetAddress serv_addr("127.0.0.1", atoi(port));
    sock->bind(&serv_addr);
    sock->listen();
    sock->setNonBlocking();
    acceptChannel = new Channel(loop, sock->getFd());
    // 非静态成员函数则需要传递this指针作为第一个参数
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setReadCallBack(cb);
    acceptChannel->enableReading();
    acceptChannel->setUseThreadPool(false);
}
Acceptor::~Acceptor() {
    delete sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    InetAddress addr;
    Socket *clnt_sock = new Socket(sock->accept(&addr));
    clnt_sock->setNonBlocking();
    newConnectionCallback(clnt_sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb) {
    newConnectionCallback = _cb;
}