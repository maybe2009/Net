//
// Created by DW on 2016/8/10.
//
#include "TransportLayerAdapter.h"

#include <netinet/in.h>
#include <iostream>

TransportLayerAdapter::TransportLayerAdapter(SocketAddress &address)
    : m_address(address)
    , m_socket(address.Domain(), address.Type(), 0)
    , m_backlog(5)
{

}

ssize_t
TransportLayerAdapter::Write(std::string& data) {
  Write(data.c_str(), data.size());
}

ssize_t
TransportLayerAdapter::Write(const void* data, size_t count) {
  try {
    return m_socket.Write(data, count);
  } catch (NetSocketException& e) {
    HandleWriteError(e);
  }
}

ssize_t
TransportLayerAdapter::Read(void *buf, size_t count) {
  try {
    return m_socket.Read(buf, count);
  } catch (NetSocketException& e) {
    HandleReadError(e);
  }
}

void
TransportLayerAdapter::Bind() {
  try {
    m_socket.Bind((const SOCKET_ADDRESS *) m_address.Address(), m_address.Size());
  } catch (NetSocketException& e) {
    HandleBindError(e);
  }
}

void
TransportLayerAdapter::Listen() {
  try {
    m_socket.Listen(m_backlog);
  } catch (NetSocketException& e) {
    HandleListenError(e);
  }
}

void TransportLayerAdapter::Connect() {
  try {
    m_socket.Connect((const SOCKET_ADDRESS *) m_address.Address(), m_address.Size());
  } catch (NetSocketException& e) {
    HandleConnectError(e);
  }
}

void
TransportLayerAdapter::Close() {
  m_socket.Close();
}

///
/// \param peer_addr pointer of the peer addres structure, Accept will store \
/// the address of peer connection here.
/// \param len size of peer_addr
/// \return new file descriptor which server will use to transfer data with client
FD TransportLayerAdapter::Accept(SOCKET_ADDRESS* peer_addr, SOCK_LEN_TYPE* len) {
  try {
    return m_socket.Accept(peer_addr, len);
  } catch (NetSocketException& e) {
    HandleAcceptError(e);
    //must throw! no useful file descriptor got
    throw;
  }
}

void
TransportLayerAdapter::SetSocketOption(int level, int opt, const void *value, SOCK_LEN_TYPE value_len) {
  if (m_socket.SetSocketOption(level, opt, value, value_len) == -1 ) {
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



