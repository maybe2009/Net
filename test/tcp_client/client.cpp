//
// Created by DW on 2016/8/8.
//

#include "NetSocket.h"

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "argc < 3" << std::endl;
    return -1;
  }

  try {
    SocketAddressV4 a0(1, std::string("192.168.1.1"));
    SocketAddressV4 a1(2, 255);
    std::cout << "ao " << a0.Ip() << std::endl;
    std::cout << "a1 " << a1.Ip() << std::endl;

    SocketAddressV4 address(1234, std::string(argv[1]));
    NetSocket socket0(AF_INET, SOCK_STREAM, 0);

    if (socket0.Connect((const SOCKET_ADDRESS*) address.Address(), address.Size()) == 0) {
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
