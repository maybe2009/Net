//
// Created by DW on 2016/8/12.
//

#include "DnsAdapter.h"
#include <iostream>

DnsAdapter::DnsAdapter(DOMAIN family,
                       TYPE type,
                       PROTOCOL protocol,
                       AI_FLAG flags)
{
  memset(&m_hint, 0, sizeof(m_hint));
  m_hint.ai_flags = flags;
  m_hint.ai_family = family;
  m_hint.ai_socktype =type;
  m_hint.ai_protocol = protocol;
}

DnsResponseList
DnsAdapter::Get(NodeName node, ServiceName service) {
  Meta *result = NULL;
  int ret = getaddrinfo(node.c_str(), service.c_str(), &m_hint, &result);
  if (ret) {
    std::string errstr = gai_strerror(ret);
    throw NetSocketException(errstr);
  }

  std::vector<DnsResponse> result_list;

  for (Meta* ptr = result; ptr != NULL; ptr = ptr->ai_next) {
    DnsResponse response;
    std::string canonical;
    if (ptr->ai_addrlen == sizeof(SOCKET_ADDRESS_V4)) {
      std::unique_ptr<SocketAddress> address(new SocketAddressV4(ptr->ai_addr));
      response.flags = ptr->ai_flags;
      response.address = std::move(address);
      if(ptr->ai_canonname != NULL) {
        response.canoname = ptr->ai_canonname;
        canonical = ptr->ai_canonname;
      } else {
        response.canoname = canonical;
      }
      response.family = ptr->ai_family;

    } else {
      //TODO: add ipv6 version
    }

    result_list.push_back(std::move(response));
  }

  freeaddrinfo(result);
  return result_list;
}
