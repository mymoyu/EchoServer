#include <strings.h>
#include <unistd.h>
#include "./include/Connection.h"
#include "./include/Channel.h"
#include "./include/Socket.h"
#include "./include/InetAddress.h"
#include "./include/Buffer.h"
#include "./include/util.h"

Connection::Connection(EventLoop* _loop, Socket *clnt_sock) : sock(clnt_sock), loop(_loop), readBuf(new Buffer()){
    // InetAddress addr;
    // sock = new Socket(serv_sock->accept(&addr));
    // sock->setNonBlocking();
    ConChannel = new Channel(loop, sock->getFd());
    ConChannel->enableReading();
    ConChannel->useET();
    
    std::function<void()> cb = std::bind(&Connection::echo, this);
    ConChannel->setReadCallBack(cb);
    ConChannel->setUseThreadPool(true);
}

Connection::~Connection() {
    delete ConChannel;
    delete sock;
    delete readBuf;
}

void Connection::echo() {
    char buf[READ_BUFSIZE];
    ssize_t read_bytes;
    int fd = sock->getFd();
    while(true) {
        bzero(&buf, sizeof(buf));
        // 读取此时的文件描述符
        read_bytes = read(fd, buf, sizeof(buf));
        if(read_bytes > 0) {
            printf("Get a message from FD_%d: %s\n", fd, buf);
            // write(fd, buf, sizeof(buf));
            readBuf->append(buf, read_bytes);
        } else if(read_bytes == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            // printf("finish reading once, errno: %d\n", errno);
            errif(write(fd, readBuf->c_str(), readBuf->size()) < 0, "write() is error\n");
            readBuf->clear();
            break;
        } else if(read_bytes == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            // close(fd);   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnectionCallback(fd);
            // delete channel;
            return ;
        } else {
            printf("Connection reset by peer\n");
            // deleteConnectionCallback(fd);
            return ;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb) {
    deleteConnectionCallback = _cb;
}