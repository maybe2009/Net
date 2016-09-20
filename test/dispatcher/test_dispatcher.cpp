//
// Created by DW on 2016/8/29.
//

#include "IOMultiplexer.h"
#include "NetSocket.h"

#include <sys/epoll.h>

#include <iostream>

void foo() {
  std::cout << "Say Hello!" << std::endl;
}

int main () {
  try {
    //set up server sockets
    SocketAddressV4 address(1234, std::string("127.0.0.1"));
    NetSocket socket0(AF_INET, SOCK_STREAM, 0);
    socket0.Bind((const SOCKET_ADDRESS *) address.Address(), address.Size());

    std::cout << "Listening..." << std::endl;
    socket0.Listen(5);

    int _socket = socket0.FileDescriptor();

    std::cout << "Create Event..." << std::endl;
    LinuxEvent event0(_socket, EPOLLIN | EPOLLOUT | EPOLLRDHUP);

    std::cout << "Creae Subscriber" << std::endl;
    EventSubscriber subscriber(_socket, READ);
    MultiplexerCallBack callBack(foo);
    subscriber.SetCallBack(callBack);

    std::cout << "Create IOMultiplexer..." << std::endl;
    Epoll dispatcher;
    IOMultiplexer multiplexer(dispatcher);

    std::cout << "Add event0 now..." << std::endl;
    multiplexer.AddEvent(event0);

    std::cout << "Subscribe now..." << std::endl;
    multiplexer.Subscribe(subscriber);

    std::cout << "Wait now..." << std::endl;
    multiplexer.Run();

    std::cout << "Over" << std::endl;

    return 0;
  } catch (std::exception &e) {
    std::cout << "main catch: " << e.what() << std::endl;
  }
}