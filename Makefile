server:
	g++ client.cpp -o client && \
	g++ server.cpp src/Server.cpp src/EventLoop.cpp src/Channel.cpp src/util.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Acceptor.cpp src/Connection.cpp src/Buffer.cpp src/ThreadPool.cpp -lpthread -o server
threadtest:
	g++ ThreadPollTest.cpp src/ThreadPoll.cpp -lpthread -o threadtest
test:
	g++ test.cpp src/util.cpp src/Buffer.cpp src/InetAddress.cpp src/Socket.cpp src/ThreadPool.cpp -lpthread -o test
clean:
	rm server && rm client

# g++ util.cpp client.cpp -o client && \