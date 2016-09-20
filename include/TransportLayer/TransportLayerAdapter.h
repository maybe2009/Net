//
// Created by DW on 2016/8/10.
//
#ifndef NET_TRANSPORTLAYERADAPTER_H
#define NET_TRANSPORTLAYERADAPTER_H

#include "NetSocket.h"
#include "../Utility/StringHelpers.h"

#include <string>

class TransportLayerAdapter {
public:
  TransportLayerAdapter(SocketAddress &address);

  ssize_t Write(std::string& data);

  ssize_t Write(const void* data, size_t count);

  ssize_t Read(void *buf, size_t count);

  void  Bind();

  void  Listen();

  void  Connect();

  void  Close();

  FD    Accept(SOCKET_ADDRESS* peer_addr, SOCK_LEN_TYPE* len);

  void  SetSocketOption(int level, int opt, const void *value,
                        SOCK_LEN_TYPE value_len);
protected:
  virtual void  HandleBindError(std::exception&);

  virtual void  HandleListenError(std::exception&);

  virtual void  HandleConnectError(std::exception&);

  virtual void  HandleAcceptError(std::exception&);

  virtual void  HandleReadError(std::exception&);

  virtual void  HandleWriteError(std::exception&);

private:
  NetSocket m_socket;
  SocketAddress &m_address;
  int m_backlog;
};

#endif //NET_TRANSPORTLAYERADAPTER_H
