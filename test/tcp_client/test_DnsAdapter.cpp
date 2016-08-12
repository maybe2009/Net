//
// Created by DW on 2016/8/12.
//

#include "DnsAdapter.h"

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "argc < 3" << std::endl;
    return -1;
  }

  std::string domain_name(argv[1]);
  std::string service_name(argv[2]);

  try {
    DnsAdapter dns(AF_INET, SOCK_STREAM, 0, AI_CANONNAME);
    DnsResponseList list = dns.Get(domain_name, service_name);

    for (auto &&item : list) {
      std::cout << "********************************" << std::endl;

      std::cout << "Canonical name: " << item.canoname << std::endl;
      std::cout << "Ip: " << item.address->Ip() << std::endl;
      std::cout << "Port: " << item.address->Port() << std::endl;

      std::cout << "********************************" << std::endl;
    }
  } catch (std::exception &e) {
    e.what();
  }

  return 0;
}