#ifndef INET_ADDRESS_H__
#define INET_ADDRESS_H__
#include <arpa/inet.h>

class InetAddress
{
public:
    sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();
};

#endif