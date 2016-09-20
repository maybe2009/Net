//
// Created by DW on 2016/8/11.
//
#include "TransportLayerAdapter.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "argc < 3" << std::endl;
    return -1;
  }

  try {
    SocketAddressV4 address(1234, std::string(argv[1]));
    TransportLayerAdapter server(address);
    server.Bind();
    server.Listen();

    int new_fd;
    std::string greeting = "Hello, client!";
    char response[256];
    memset(response, 0, 256);

    if ((new_fd = server.Accept(NULL, NULL)) != -1) {
      if (::write(new_fd, greeting.c_str(), greeting.size()) > 0) {
        std::cout << "Write Success" << std::endl;

        int count = 0;
        if ((count = ::read(new_fd, response, 256)) > 0) {
          std::cout << "Read Success" << std::endl;
          response[count] = 0;
          puts(response);
        } else {
          perror("Read: ");
        }

      } else {
        std::cout << "Write Error" << std::endl;
        perror("Write: ");
      }
    }
  } catch (NetSocketException& e) {
    std::cout << e.what() << std::endl;

  }
  return 0;
}
