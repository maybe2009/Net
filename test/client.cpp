//
// Created by DW on 2016/8/8.
//
#include "NetSocket.h"

#include <fcntl.h>

#include <iostream>
#include <string>

int main() {
  try {
    SocketAddressV4 address(1234, std::string("127.0.0.1"));
    NetSocket socket0(AF_INET, SOCK_STREAM, 0);

    if (socket0.Connect((const ADDR*)address.Get(), address.Size()) == 0) {
      std::cout << "Connect Success "  << std::endl;
    } else {
      std::cout << "Connect Error " << std::endl;
    }
  }
  catch (NetSocketException& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
