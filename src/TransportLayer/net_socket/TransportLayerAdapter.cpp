//
// Created by DW on 2016/8/10.
//
#include "TransportLayerAdapter.h"
#include <netinet/in.h>

TransportLayerAdapter::TransportLayerAdapter(
    in_port_t port,
    std::string ip,
    DOMAIN domain,
    TYPE type,
    PROTOCOL protocol)
    : m_address(port, ip)
    , m_connection(domain, type, protocol)
{}

TransportLayerAdapter::TransportLayerAdapter(
    in_port_t port,
    uint32_t ip,
    DOMAIN domain,
    TYPE type,
    PROTOCOL protocol)
    : m_address(port, ip)
    , m_connection(domain, type, protocol)
{}


ssize_t
TransportLayerAdapter::Write(std::string& data) {
  try {
    return m_connection.Write(data.c_str(), data.size());
  } catch (NetSocketException& e) {
    HandleWriteError(e);
  }

}

ssize_t
TransportLayerAdapter::Write(const void* data, size_t count) {
  try {
    return m_connection.Write(data, count);
  } catch (NetSocketException& e) {
    HandleWriteError(e);
  }
}

ssize_t
TransportLayerAdapter::Read(void *buf, size_t count) {
  try {
    return m_connection.Read(buf, count);
  } catch (NetSocketException& e) {
    HandleReadError(e);
  }
}

void
TransportLayerAdapter::Bind() {
  try {
    m_connection.Bind((const ADDR *) m_address.Get(), m_address.Size());
  } catch (NetSocketException& e) {
    HandleBindError(e);
  }
}

void
TransportLayerAdapter::Listen(int backlog) {
  try {
    m_connection.Listen(backlog);
  } catch (NetSocketException& e) {
    HandleListenError(e);
  }
}

void TransportLayerAdapter::Connect() {
  try {
    m_connection.Connect((const ADDR *) m_address.Get(), m_address.Size());
  } catch (NetSocketException& e) {
    HandleConnectError(e);
  }
}

FD TransportLayerAdapter::Accept(ADDR* peer_addr, SOCK_LEN_TYPE* len) {
  try {
    return m_connection.Accept(peer_addr, len);
  } catch (NetSocketException& e) {
    HandleAcceptError(e);
  }
}

void
TransportLayerAdapter::HandleAcceptError(std::exception& exception) {
  throw;
}
void
TransportLayerAdapter::HandleConnectError(std::exception& exception) {
  throw;
}

void
TransportLayerAdapter::HandleListenError(std::exception& exception) {
  throw;
}

void
TransportLayerAdapter::HandleBindError(std::exception& exception) {
  throw;
}

void
TransportLayerAdapter::HandleReadError(std::exception& exception) {
  throw;
}

void
TransportLayerAdapter::HandleWriteError(std::exception& exception) {
  throw;
}