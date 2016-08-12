//
// Created by DW on 2016/8/12.
//

#ifndef NET_DNSADAPTER_H
#define NET_DNSADAPTER_H

#include "NetSocket.h"

#include <netdb.h>
#include <string>
#include <ws2tcpip.h>
using  namespace std;

typedef AI_FLAG int;

class DnsAdapter {
  typedef struct addrinfo Meta;
  typedef NodeName std::string;
  typedef ServiceName std::string;
public:
  DnsAdapter(DOMIAN, TYPE, PROTOCOL, AI_FLAG);
  Meta Get(NodeName, ServiceName);

private:
  Meta m_hint;
  Meta m_response;
};

#endif //NET_DNSADAPTER_H
