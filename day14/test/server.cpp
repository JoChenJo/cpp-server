#include "EventLoop.h"
#include "Server.h"
#include "Connection.h"
#include <iostream>
#include "Socket.h"
#include "Buffer.h"


int main(){
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    server->OnConnect([](Connection *conn) {
    conn->Read();
    if (conn->GetState() == Connection::State::Closed) {
      conn->Close();
      return;
    }
    std::cout << "Message from client " << conn->GetSocket()->getFD() << ": " << conn->ReadBuffer() << std::endl;
    conn->SetSendBuffer(conn->ReadBuffer());
    conn->Write();
  });

    loop->loop();
    delete loop;
    delete server;
    return 0;
}