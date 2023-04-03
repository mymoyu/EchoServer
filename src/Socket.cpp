#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "./include/InetAddress.h"
#include "./include/Socket.h"
#include "./include/util.h"

Socket::Socket() {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd < 0, "socket()");
}
Socket::Socket(int _fd) : fd(_fd) {
    errif(fd < 0, "socket()");
}

Socket::~Socket() {
    if(fd >= 0) {
        close(fd);
    }
}

void Socket::bind(InetAddress* addr) {
    errif(::bind(fd, (sockaddr*)&(addr->addr), addr->addr_len) < 0, "bind()");
}

void Socket::listen() {
    errif(::listen(fd, 5) < 0, "listen()");
}

int Socket::accept(InetAddress* adr) {
    int clnt_fd = ::accept(fd, (sockaddr*)&(adr->addr), &(adr->addr_len));
    errif(clnt_fd < 0, "accpet()");
    printf("the client's fd is %d, IP: %s , Port: %d \n", clnt_fd, inet_ntoa(adr->addr.sin_addr), ntohs(adr->addr.sin_port));
    return clnt_fd;
}

// 解除阻塞IO, 变为非阻塞IO
void Socket::setNonBlocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void Socket::connect(InetAddress* adr) {
    errif( ::connect(fd, (sockaddr*)&(adr->addr), adr->addr_len) < 0, "connect() is error");
}