//
// Created by DW on 2016/8/12.
//

#ifndef NET_DNSADAPTER_H
#define NET_DNSADAPTER_H

#include "NetSocket.h"

#include <netdb.h>
#include <string>
#include <vector>
#include <memory>

typedef int AI_FLAG;

struct DnsResponse {
  AI_FLAG flags;
  std::unique_ptr<SocketAddress> address;
  std::string canoname;
  DOMAIN family;
  DnsResponse() = default;

  DnsResponse(DnsResponse&& rhs) {
    this->flags = rhs.flags;
    this->address = std::move(rhs.address);
    this->canoname = rhs.canoname;
  }

  DnsResponse& operator=(DnsResponse& rhs) {
    this->flags = rhs.flags;
    this->address = std::move(rhs.address);
    this->canoname = rhs.canoname;
    return *this;
  }
};

typedef std::vector<DnsResponse> DnsResponseList;

class DnsAdapter {
  typedef struct addrinfo Meta;
  typedef std::string NodeName;
  typedef std::string ServiceName;
public:
  DnsAdapter(DOMAIN, TYPE, PROTOCOL, AI_FLAG);
  DnsResponseList Get(NodeName, ServiceName);

private:
  Meta m_hint;
};

#endif //NET_DNSADAPTER_H
