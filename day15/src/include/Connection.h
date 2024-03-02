#pragma once
#include <functional>
#include "Macros.h"
class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection
{
public:
    enum State{
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };
public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();
    DISALLOW_COPY_AND_MOVE(Connection);

    void Read();
    void Write();
    void Send(std::string msg);

    void SetDeleteConnectionCallback(std::function<void(Socket *)> const &callback);
    void SetOnConnectCallback(std::function<void(Connection *)> const &callback);
    void SetOnMessageCallback(std::function<void(Connection *)> const &callback);
    void Business();

    State GetState();
    void Close();
    void SetSendBuffer(const char *);
    Buffer *GetReadBuffer();
    const char *ReadBuffer();
    Buffer *GetSendBuffer();
    const char *SendBuffer();
    void GetlineSendBuffer();
    Socket *GetSocket();

    void OnConnect(std::function<void()> fn);
    void OnMessage(std::function<void()> fv);

private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    State state;
    Buffer *read_buffer;
    Buffer *send_buffer;
    std::function<void(Socket *)> delete_connection_callback;
    std::function<void(Connection *)> on_connect_callback;
    std::function<void(Connection *)> on_message_callback;

    void ReadNonBlocking();
    void WriteNonBlocking();
    void ReadBlocking();
    void WriteBlocking();
};