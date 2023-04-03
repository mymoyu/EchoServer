#include <memory>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <functional>
#include <thread>
#include "./include/Server.h"
#include "./include/Socket.h"
#include "./include/Acceptor.h"
#include "./include/Epoll.h"
#include "./include/Channel.h"
#include "./include/InetAddress.h"
#include "./include/util.h"
#include "./include/Connection.h"
#include "./include/ThreadPool.h"
#include "./include/EventLoop.h"

#define READ_BUFSIZE 1024

Server::Server(EventLoop* _loop, const char *port) : mainReactor(_loop) {
    acceptor = new Acceptor(mainReactor, port);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    int size = std::thread::hardware_concurrency(); // 线程数目来限制subReactor的数量
    thpool = new ThreadPool(size);
    for (int i = 0; i < size; ++i)
    {
        subReactors.push_back(new EventLoop());
    }
    for (int i = 0; i < size; ++i)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]); // 事件绑定线程
        thpool->add(sub_loop); // 开启所有线程进入负责事件处理的状态
    }
}

Server::~Server() {
    delete acceptor;
    // delete mainReactor;
    delete thpool;
}

// void Server::handleReadEvent(Channel* channel) {
//     int fd = channel->getFd();
//     char buf[READ_BUFSIZE];
//     ssize_t read_bytes;
//     while(true) {
//         bzero(&buf, sizeof(buf));
//         // 读取此时的文件描述符
//         read_bytes = read(fd, buf, sizeof(buf));
//         if(read_bytes > 0) {
//             printf("Get a message from FD_%d: %s\n", fd, buf);
//             write(fd, buf, sizeof(buf));
//         } else if(read_bytes == -1 && errno == EINTR){  //客户端正常中断、继续读取
//             printf("continue reading");
//             continue;
//         } else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
//             printf("finish reading once, errno: %d\n", errno);
//             break;
//         } else if(read_bytes == 0){  //EOF，客户端断开连接
//             printf("EOF, client fd %d disconnected\n", fd);
//             close(fd);   //关闭socket会自动将文件描述符从epoll树上移除
//             delete channel;
//             return ;
//         }
//     }
// }

void Server::newConnection(Socket* clnt_sock) {
    if(clnt_sock->getFd() != -1) {
        // hash分配事件给特定的线程
        int random = clnt_sock->getFd() % subReactors.size();
        Connection *conn = new Connection(subReactors[random], clnt_sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[clnt_sock->getFd()] = conn;
    }

    // std::shared_ptr<InetAddress> clnt_addr = std::make_shared<InetAddress>();
    // int clnt_fd = ::accept(serv_sock->getFd(), (sockaddr*)&(clnt_addr->addr), &(clnt_addr->addr_len));
    // errif(clnt_fd < 0, "accpet()");
    // printf("the client's fd is %d, IP: %s , Port: %d \n", clnt_fd, inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    // // 转为非阻塞IO
    // fcntl(clnt_fd, F_SETFL, fcntl(clnt_fd, F_GETFL) | O_NONBLOCK);

    // std::shared_ptr<Channel> clntChannel = std::make_shared<Channel>(ep, clnt_fd);
    // Channel* clntChannel = new Channel(loop, clnt_fd);
    // std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clntChannel);
    // clntChannel->setCallBack(cb);
    // clntChannel->enableReading();
}

void Server::deleteConnection(int sockfd) {
    if(sockfd != -1) {
        auto it = connections.find(sockfd);
        if(it != connections.end()) { // 如果存在这个连接则释放
            Connection* conn = connections[sockfd];
            connections.erase(sockfd);
            // 回收空间
            delete conn;
        }
    }
}