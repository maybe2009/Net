//
// Created by DW on 2016/8/10.
//
#ifndef NET_TRANSPORTLAYERADAPTER_H
#define NET_TRANSPORTLAYERADAPTER_H

#include "NetSocket.h"

#include <string>

class TransportLayerAdapter {
public:
  TransportLayerAdapter(in_port_t port, uint32_t ip, DOMAIN, TYPE, PROTOCOL);
  TransportLayerAdapter(in_port_t port, std::string ip, DOMAIN, TYPE, PROTOCOL);

  ssize_t Write(std::string& data);
  ssize_t Write(const void* data, size_t count);
  ssize_t Read(void *buf, size_t count);

  void  Bind();
  void  Listen(int backlog);
  void  Connect();
  FD    Accept(SOCKET_ADDRESS* peer_addr, SOCK_LEN_TYPE* len);

  void  SetSocketOption(int levle, int opt, const void *value, SOCK_LEN_TYPE value_len);
protected:
  virtual void  HandleBindError(std::exception& );
  virtual void  HandleListenError(std::exception&);
  virtual void  HandleConnectError(std::exception&);
  virtual void  HandleAcceptError(std::exception&);
  virtual void  HandleReadError(std::exception&);
  virtual void  HandleWriteError(std::exception&);

 private:
  NetSocket m_connection;
  SocketAddressV4 m_address;
};

#endif //NET_TRANSPORTLAYERADAPTER_H
