//
// Created by DW on 2016/8/29.
//

#include "IOMultiplexer.h"
#include "NetSocket.h"

#include <sys/epoll.h>

#include <iostream>

int main () {
  //set up server sockets
  SocketAddressV4 address(1234, std::string("127.0.0.1"));
  NetSocket socket0(AF_INET, SOCK_STREAM, 0);
  socket0.Bind((const SOCKET_ADDRESS*)address.Get(), address.Size());
  std::cout << "Listening..." << std::endl;
  socket0.Listen(5);

  IOMultiplexer multiplexer(36);

  std::cout << "Add a event now..." << std::endl;

  //create Event and add to IOMultiplexer
  Event event(socket0.GetSocket(),  EPOLLIN|EPOLLOUT|EPOLLRDHUP );
  multiplexer.AddEvent(event);

  std::cout << "Dispatch it now..." << std::endl;
  multiplexer.Dispatch();

  return 0;
}