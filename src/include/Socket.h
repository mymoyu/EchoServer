#ifndef SOCKET_H__
#define SOCKET_H__
class InetAddress;

#include <memory>

class Socket
{
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

public:
    void bind(InetAddress*);
    void listen();
    int accept(InetAddress*);
    
public:
    void connect(InetAddress*);
    void setNonBlocking();

    int getFd() const { return fd; }
};

#endif