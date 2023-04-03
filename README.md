# EchoServer -- 回声服务器

#### 文件结构:

.
├── src
│       ├── Acceptor.cpp  
│       ├── Buffer.cpp  
│       ├── Channel.cpp  
│       ├── Connection.cpp  
│       ├── Epoll.cpp  
│       ├── EventLoop.cpp  
│       ├── InetAddress.cpp  
│       ├── Server.cpp  
│       ├── Socket.cpp  
│       ├── ThreadPool.cpp  
│       ├── util.cpp  
│       └── include  
│               ├── Acceptor.h  
│               ├── Buffer.h  
│               ├── Channel.h  
│               ├── Connection.h  
│               ├── Epoll.h  
│               ├── EventLoop.h  
│               ├── InetAddress.h  
│               ├── Server.h  
│               ├── Socket.h  
│               ├── ThreadPool.h  
│               └── util.h  
├── server.cpp  
├── client.cpp  
├── Makefile  
└── README.md  

#### 程序功能解析:

###### 建立网络连接和接收发送功能

Acceptor （创建Socket，建立客户端的连接事件）  

Socket（创建和管理socket，封装linux网络API）  

InetAddress（封装sockaddr_in结构体，供socket填充信息）  

Connection（初始化连接信息，实现服务器的接收请求和发送请求，在本项目为回声功能）  

Buffer（信息缓存，处理字符串）  

###### 线程池功能

ThreadPool（线程池，利用抢占式提升线程获取任务的速度，提高执行效率）  

###### Reactor

Epoll（封装epoll，管理epoll的主要功能）  

Channel（封装事件，作为被epoll监听的子任务）  

EventLoop（管理epoll以及小型线程池【处理监听到的请求】，是主从Reactor模式的核心）  

Acceptor（将连接后的连接通过哈希的方式交付给从属的Reactor）  

###### 主从Reactor多线程

Server 功能如下

1. 创建主Reactor，将Acceptor交由Reactor管理，创建于线程数相同的从Reactor和从Reactor数组以及线程池，将从Reactor交付到线程池中

2. 维护连接事件channel的哈希表，实现连接删除。

###### 基础功能【util】

errif（打印日志）