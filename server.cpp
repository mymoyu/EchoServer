#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <memory>
#define BUFSIZE 1024
#define MAX_EVENTS 1024
#include "./src/include/EventLoop.h"
#include "./src/include/Server.h"

static void echo(Channel* channel);

int main(int argc, char **argv)
{
    int i;
    // nfds 为此时被监控到的事件数目
    int epfd, nfds;

    if(argc != 2)
    {
        fprintf(stderr, "error: %s <port>", argv[0]);
        exit(0);
    }

    EventLoop loop;
    std::unique_ptr<Server> server = std::make_unique<Server>(&loop, argv[1]);
    loop.loop();
   
    exit(1);
}