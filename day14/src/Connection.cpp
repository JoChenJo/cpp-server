#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "util.h"
#include "Buffer.h"

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <utility>

Connection::Connection(EventLoop *_loop, Socket *_sock):loop(_loop), sock(_sock), channel(nullptr), 
read_buffer(nullptr), send_buffer(nullptr), state(State::Invalid){
    if(loop != nullptr){
        channel = new Channel(loop, sock->getFD());
        channel->enableRead();
        channel->useET();
    }
    read_buffer = new Buffer();
    send_buffer = new Buffer();
    state = State::Connected;
}

Connection::~Connection(){
    delete channel;
    delete sock;
    delete read_buffer;
    delete send_buffer;
}

void Connection::Read(){
    ASSERT(state == State::Connected, "connection state is disconnectioned!");
    read_buffer->clear();
    if(sock->IsNonBlocking()){
        ReadNonBlocking();
    }else {
        ReadBlocking();
    }
}

void Connection::Write() {
  ASSERT(state == State::Connected, "connection state is disconnected!");
  if (sock->IsNonBlocking()) {
    WriteNonBlocking();
  } else {
    WriteBlocking();
  }
  send_buffer->clear();
}

void Connection::ReadNonBlocking(){
    int sockfd = sock->getFD();
    char buf[1024];
    while(true){
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            read_buffer->append(buf, bytes_read);
        }else if(bytes_read == -1 && errno == EINTR){   //代表程序正常中断，继续读取
            printf("continue reading\n");
        }else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){    //非阻塞IO，这个条件表示数据全部读取完毕
            break;
        }else if(bytes_read == 0){
            printf("read EOF, client fd %d disconnected\n", sockfd);
            state = State::Closed;
            break;
        }else {
            printf("Other error on client fd %d\n", sockfd);
            state = State::Closed;
            break;
        }
    }
}

void Connection::WriteNonBlocking(){
    int sockfd = sock->getFD();
  char buf[send_buffer->size()];
  memcpy(buf, send_buffer->c_str(), send_buffer->size());
  int data_size = send_buffer->size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EINTR) {
      printf("continue writing\n");
      continue;
    }
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    if (bytes_write == -1) {
      printf("Other error on client fd %d\n", sockfd);
      state = State::Closed;
      break;
    }
    data_left -= bytes_write;
  }
}

void Connection::ReadBlocking() {
  int sockfd = sock->getFD();
  unsigned int rcv_size = 0;
  socklen_t len = sizeof(rcv_size);
  getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
  char buf[rcv_size];
  ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
  if (bytes_read > 0) {
    read_buffer->append(buf, bytes_read);
  } else if (bytes_read == 0) {
    printf("read EOF, blocking client fd %d disconnected\n", sockfd);
    state = State::Closed;
  } else if (bytes_read == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state = State::Closed;
  }
}

void Connection::WriteBlocking() {
  // 没有处理send_buffer_数据大于TCP写缓冲区，的情况，可能会有bug
  int sockfd = sock->getFD();
  ssize_t bytes_write = write(sockfd, send_buffer->c_str(), send_buffer->size());
  if (bytes_write == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state = State::Closed;
  }
}

void Connection::Close() { delete_connection_callback(sock); }

Connection::State Connection::GetState() { return state; }
void Connection::SetSendBuffer(const char *str) { send_buffer->setBuf(str); }
Buffer *Connection::GetReadBuffer() { return read_buffer; }
const char *Connection::ReadBuffer() { return read_buffer->c_str(); }
Buffer *Connection::GetSendBuffer() { return send_buffer; }
const char *Connection::SendBuffer() { return send_buffer->c_str(); }

void Connection::SetDeleteConnectionCallback(std::function<void(Socket *)> const &callback){
    delete_connection_callback = callback;
}

void Connection::SetOnConnectCallback(std::function<void(Connection *)> const &callback) {
  on_connect_callback = callback;
  channel->setReadCallback([this]() { on_connect_callback(this); });
}

void Connection::GetlineSendBuffer() { send_buffer->getline(); }

Socket *Connection::GetSocket() { return sock; }