#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <exception>
#include <cstring>
#include <string>

#define MAX_SIZE 256

typedef int FD;
typedef int DOMAIN;
typedef int TYPE;
typedef int PROTOCOL;
typedef struct sockaddr ADDR;
typedef socklen_t SOCK_LEN_TYPE;
typedef int FLAG;

class NetSocket
{
private:
  void CreateSocket(DOMAIN, TYPE, PROTOCOL);

public:
  void  Bind(const ADDR* addr, SOCK_LEN_TYPE len);
  void  Listen(int backlog);
  int   Connect(const ADDR* addr, SOCK_LEN_TYPE len);
  int   Accept(ADDR*, SOCK_LEN_TYPE*);

public:
  NetSocket(DOMAIN, TYPE, PROTOCOL);

private:
  FD m_fd;
};

class NetSocketException : public std::exception {
public:
  NetSocketException(int);
  virtual const char* what() const noexcept override ;

private:
  int m_errno;
  char m_errstr[MAX_SIZE];
};

class SocketAddressV4 {
public:
  SocketAddressV4(in_port_t port, uint32_t ip) {
    memset(&m_addr, 0, Size());
    m_addr.sin_port = port;
    m_addr.sin_addr.s_addr = ip;
    m_addr.sin_family = AF_INET;
  }

  SocketAddressV4(in_port_t port, std::string ip) {
    memset(&m_addr, 0, Size());
    m_addr.sin_port = port;
    m_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip.c_str(), &(m_addr.sin_addr)) != 1) {
      throw NetSocketException(errno);
    }
  }

  size_t Size() const {
    return sizeof(m_addr);
  }

  const struct sockaddr_in* Get() const {
    return &m_addr;
  }

private:
  struct sockaddr_in m_addr;
};

#endif /* NET_SOCKET_H */
