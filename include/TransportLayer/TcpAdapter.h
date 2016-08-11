//
// Created by DW on 2016/8/11.
//

#ifndef NET_TCPADAPTER_H
#define NET_TCPADAPTER_H

#include "TransportLayerAdapter.h"
#include <netinet/in.h>

class TcpAdapter : public  TransportLayerAdapter {
public:
  TcpAdapter(in_port_t port, std::string ip, DOMAIN A);
  TcpAdapter(in_port_t port, uint32_t ip, DOMAIN a);
};

#endif //NET_TCPADAPTER_H
