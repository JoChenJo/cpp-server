server:
	g++ src/util.cpp client.cpp src/InetAddress.cpp src/Socket.cpp src/Buffer.cpp -o client && \
	g++ server.cpp \
	-pthread \
	src/util.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Connection.cpp \
	src/Channel.cpp src/EventLoop.cpp src/Server.cpp src/Acceptor.cpp  src/Buffer.cpp \
	src/ThreadPool.cpp \
	-o server
clean:
	rm server && rm client

threadText:
	g++ -pthread src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest