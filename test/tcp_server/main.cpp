//
// Created by DW on 2016/8/8.
//
#include "NetSocket.h"

#include <unistd.h>
#include <iostream>

int main() {
  try {
    SocketAddressV4 address(1234, std::string("127.0.0.1"));
    NetSocket socket0(AF_INET, SOCK_STREAM, 0);
    socket0.Bind((const SOCKET_ADDRESS*) address.Address(), address.Size());
    std::cout << "Listening..." << std::endl;
    socket0.Listen(5);

    if (FD client_fd = socket0.Accept(NULL, NULL)) {
      std::cout << "New fd " << client_fd << std::endl;
      close(client_fd);
    } else {
      std::cout << "Accept Error " << client_fd << std::endl;
    }
  }
  catch (NetSocketException& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}