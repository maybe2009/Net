//
// Created by DW on 2016/8/11.
//

#include "TcpAdapter.h"

TcpAdapter::TcpAdapter(ip_port_t port, std::string ip, DOMAIN domain)
  : TransportLayerAdapter(port, ip, domain, SOCK_STREAM, 0)
{
}
