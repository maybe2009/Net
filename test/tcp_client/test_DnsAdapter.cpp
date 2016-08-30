//
// Created by DW on 2016/8/12.
//

#include "DnsAdapter.h"
#include "FixedBuffer.h"
#include "HttpRequest.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <TcpAdapter.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <arpa/inet.h>
#include <netinet/tcp.h>


int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cout << "argc < 3" << std::endl;
    return -1;
  }

  try {
    std::string domain_name(argv[1]);
    std::string service_name(argv[2]);
    std::string url(argv[3]);

    std::cout << "Query: " << std::endl;
    std::cout << "Domain " << domain_name << std::endl;
    std::cout << "Service " << service_name << std::endl;
    std::cout << "Url " << url << std::endl;

    DnsAdapter dns(AF_INET, SOCK_STREAM, 0, AI_CANONNAME);
    DnsResponseList list = dns.Get(domain_name, service_name);

    HttpRequest http_request;
    HeaderMap map;
    Body body;
    //Url url = "http://www.baidu.com/";
    //map["Accept"] = "text/html";
    //map["User-agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.82 Safari/537.36";
    //map["Upgrade-Insecure-Requests"] = "1";

    for (auto &&item : list) {
      std::string request0 = http_request.HttpGet(url, map, body);

      std::cout << "********************************" << std::endl;
      std::cout << "Canonical name: " << item.canoname << std::endl;
      std::cout << "Ip: " << item.address->Ip() << std::endl;
      std::cout << "Port: " << ntohs(item.address->Port()) << std::endl;
      std::cout << "********************************" << std::endl;
      std::cout << "Request is " << request0 << std::endl;
      std::cout << "Trying to connect.... " << std::endl;

      try {
        TcpAdapter conn(item.address->Port(), item.address->Ip(), item.address->Family());

        int on = 1;
        conn.SetSocketOption(IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(int));

        conn.Connect();
        //conn.Write("GET / \r\n", sizeof("GET /\r\n"));
        conn.Write(request0);
        char buf[2048];
        conn.Read(buf, 2048);
        buf[2048] = 0;
        std::cout << "Replys is : " << buf << std::endl;

      } catch (NetSocketException &e) {
        std::cout << e.what() << std::endl;
        continue;
      }
    }
  } catch (std::exception &e) {
    std::cout << "Exception:" << std::endl;
    std::cout << e.what() << std::endl;

  }

  return 0;
}