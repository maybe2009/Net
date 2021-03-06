//
// Created by DW on 2016/9/19.
//

#include "Connection.h"

#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "argc < 2" << std::endl;
    return -1;
  }

  std::string ip = argv[1];

  SocketAddressV4 address(1234, ip);

  TcpClient client(address);
  client.Open();
  std::cout << "Open success" << std::endl;

  std::string say = "Hello from TcpClient";
  TcpBuffer buffer(say.size());
  buffer.Fill(say.c_str(), say.size());

  client.tx(buffer);
  std::cout << "send success" << std::endl;

  client.rx(buffer);

  return 0;
}
