//
// Created by DW on 2016/8/10.
//
#include "TransportLayerAdapter.h"
#include <netinet/in.h>
#include <iostream>

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
  Write(data.c_str(), data.size());
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
    m_connection.Bind((const SOCKET_ADDRESS *) m_address.Get(), m_address.Size());
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
    m_connection.Connect((const SOCKET_ADDRESS *) m_address.Get(), m_address.Size());
  } catch (NetSocketException& e) {
    HandleConnectError(e);
  }
}
///
/// \param peer_addr pointer of the peer addres structure, Accept will store \
/// the address of peer connection here.
/// \param len size of peer_addr
/// \return new file descriptor which server will use to transfer data with client
FD TransportLayerAdapter::Accept(SOCKET_ADDRESS* peer_addr, SOCK_LEN_TYPE* len) {
  try {
    return m_connection.Accept(peer_addr, len);
  } catch (NetSocketException& e) {
    HandleAcceptError(e);
    //must throw! no useful file descriptor got
    throw;
  }
}

void
TransportLayerAdapter::SetSocketOption(int level, int opt, const void *value, SOCK_LEN_TYPE value_len) {
  if (m_connection.SetSocketOption(level, opt, value, value_len) == -1 ) {
    throw NetSocketException(errno);
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

