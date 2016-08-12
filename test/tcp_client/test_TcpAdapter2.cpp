//
// Created by DW on 2016/8/11.
//

#include "TcpAdapter.h"

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "argc < 3" << std::endl;
    return -1;
  }

  try {
    TcpAdapter client(1234, std::string(argv[1]), AF_INET);

    client.Connect();
    std::cout << "Connect Success "  << std::endl;

    char response[256];
    memset(response, 0, 256);
    int size = 0;

    if ((size = client.Read(response, 256)) > 0) {
      std::cout << "Read Success" << std::endl;
      response[size] = 0;
      puts(response);

      std::string greeting = "Hello, Server!";
      if (client.Write(greeting) > 0) {
        std::cout << "Write Success" << std::endl;
      } else {
        std::cout << "Write Error" << std::endl;
        perror("Write: ");
      }

    } else {
      perror("Read: ");
    }
  }
  catch (NetSocketException& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}