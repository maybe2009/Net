#include "NetSocket.h"

#include <sys/socket.h>

NetSocket::NetSocket(int family, int type, int protocol) {
  m_fd = ::socket(family, type , protocol);
  if (m_fd == -1) {
    throw; 
  } 
}

void
NetSocket::Bind(SOCK_ADDR_TYPE* addr, SOCK_LEN_TYPE len) {
  if (::bind(m_fd, addr, len) != 0) {
    throw; 
  }
}

int 
NetSocket::Connect(SOCK_ADDR_TYPE* addr, SOCK_LEN_TYPE len) {
  if (::connect(m_fd, addr, len) != 0) {
    return -1; 
  } else {
    return 0;
  } 
}

int
NetSocket::Accept(SOCK_ADDR_TYPE* peer_addr, SOCK_LEN_TYPE* len) {
  FD_TYPE new_fd = ::accept(m_fd, peer_addr, len);
  
  if (new_fd == -1) {
    throw; 
  } 
  
  return new_fd; 
}
