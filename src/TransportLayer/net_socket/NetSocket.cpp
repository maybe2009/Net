#include "../../../include/TransportLayer/NetSocket.h"

/* put this after all the #include, so we get the POSIX version strerror_r*/
#ifdef _GNU_SOURCE
#undef _GNU_SOURCE
#endif
/* DON'T put any #include below. see http://stackoverflow.com/questions/3052041/
 * how-to-get-posix-strerror-r-instead-of-gnu-version
 */

NetSocket::NetSocket(DOMAIN family, TYPE type, PROTOCOL protocol) :
  m_fd(-1)
{
  CreateSocket(family, type, protocol);
}

void
NetSocket::CreateSocket(DOMAIN family, TYPE type, PROTOCOL protocol) {
  m_fd = ::socket(family, type, protocol);
  if (m_fd == -1) {
    throw NetSocketException(errno);
  }
}

void
NetSocket::Bind(const ADDR *addr, SOCK_LEN_TYPE len) {
  if (::bind(m_fd, addr, len) != 0) {
    throw NetSocketException(errno);
  }
}

void
NetSocket::Listen(int backlog) {
  if (::listen(m_fd, backlog) != 0) {
    throw NetSocketException(errno);
  }
}

int
NetSocket::Connect(const ADDR *addr, SOCK_LEN_TYPE len) {
  int ret = ::connect(m_fd, addr, len);
  if (ret == -1) {
    throw NetSocketException(errno);
  }
  return ret;
}

int
NetSocket::Accept(ADDR *peer_addr, SOCK_LEN_TYPE *len) {
  int new_socket = ::accept(m_fd, peer_addr, len);
  if (ret == -1) {
    throw NetSocketException(errno);
  }
  return new_socket;
}

ssize_t
NetSocket::Read(void *buf, size_t count) {
  ssize_t ret = ::read(m_fd, buf, count);
  if (ret == -1) {
    throw NetSocketException(errno);
  }
  return ret;
}

ssize_t
NetSocket::Write(const void *buf, size_t count){
  ssize_t ret = ::write(m_fd, buf, count);
  if (ret == -1) {
    throw NetSocketException(errno);
  }
  return ret;
}

NetSocketException::NetSocketException(int v)
    :m_errno(v) {
  //use GNU version strerror_r
  m_errstr = strerror_r(m_errno, m_errstr_buf, MAX_SIZE);
  //for safety, ensure m_errstr_buf has at least one null-terminator
  m_errstr_buf[MAX_SIZE] = 0;
}

NetSocketException::NetSocketException(std::string& str){
  m_errstr = str;
}

const char *NetSocketException::what() const noexcept {
  return m_errstr.c_str();
}