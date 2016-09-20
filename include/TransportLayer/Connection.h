//
// Created by DW on 2016/9/19.
//

#ifndef NET_CONNECTION_H
#define NET_CONNECTION_H

#include "TransportLayerAdapter.h"
#include "../Utility/StringHelpers.h"

#include <string>

typedef UniquePtrArray<char> TcpBuffer;

class Connection {
public:
  virtual void Open() = 0;
  virtual void Close() = 0;
  virtual void rx(TcpBuffer &str) = 0;
  virtual void tx(TcpBuffer &str) = 0;
};

class TcpClient : public Connection {
public:
  TcpClient(SocketAddress &address)
      : m_adapter(address)
  {}

public:
  void Open() override {
    m_adapter.Connect();
  }

  void Close() override {
    m_adapter.Close();
  }

  void rx(TcpBuffer &buf) override {
    m_adapter.Read(buf.Raw(), buf.Size());
  }

  void tx(TcpBuffer &data) override {
    m_adapter.Write(data.Raw(), data.Size());
  }

protected:
  TransportLayerAdapter m_adapter;
};

class TcpServer : public Connection {
public:
  TcpServer(SocketAddress &address)
      : m_adapter(address)
  {}

public:
  void Open() override {
    m_adapter.Bind();
    m_adapter.Listen();
  }

  void Close() override {
    m_adapter.Close();
  }

  void rx(TcpBuffer &buf) override {
    m_adapter.Read(buf.Raw(), buf.Size());
  }

  void tx(TcpBuffer &data) override {
    m_adapter.Write(data.Raw(), data.Size());
  }

private:
  TransportLayerAdapter m_adapter;
};

#endif //NET_CONNECTION_H
