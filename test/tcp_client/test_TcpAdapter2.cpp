//
// Created by DW on 2016/8/11.
//

#include "TransportLayerAdapter.h"

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "argc < 3" << std::endl;
    return -1;
  }

  try {
    SocketAddressV4 address(1234, std::string(argv[1]));
    TransportLayerAdapter client(address);
    client.Bind();
    //client.Connect();
    std::cout << "Listen" << std::endl;
      
    client.Listen();
    std::cout << "Connect Success "  << std::endl;

    char response[256];
    memset(response, 0, 256);
    int size = 0;

    std::string say = "Hello Myself";
    std::cout << "Write begin" << std::endl;
    client.Write(say);
    std::cout << "Write over" << std::endl;
      
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
    std::cout << "Exception: " << e.what()  << " err: " << e.ErrorNumber()
              << std::endl;
  }
  std::cout << "bye" << std::endl;
    
  return 0;
}