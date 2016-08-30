#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "NetExeceptions.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <exception>
#include <cstring>
#include <string>

#define MAX_SIZE 256

typedef int FD;
typedef int DOMAIN;
typedef int TYPE;
typedef int PROTOCOL;
typedef socklen_t SOCK_LEN_TYPE;
typedef int FLAG;
typedef struct sockaddr SOCKET_ADDRESS;
typedef struct sockaddr_in SOCKET_ADDRESS_V4;
typedef struct sockaddr_in6 SOCKET_ADDRESS_V6;

class NetSocket
{
private:
  void CreateSocket(DOMAIN, TYPE, PROTOCOL);

public:
  void      Bind(const SOCKET_ADDRESS* addr, SOCK_LEN_TYPE len);
  void      Listen(int backlog);
  int       Connect(const SOCKET_ADDRESS* addr, SOCK_LEN_TYPE len);
  int       Accept(SOCKET_ADDRESS*, SOCK_LEN_TYPE*);
  ssize_t   Read(void *buf, size_t count);
  ssize_t   Write(const void *buf, size_t count);
  FD        GetSocket() const;
  int       SetSocketOption(int level, int opt, const void* value, SOCK_LEN_TYPE value_len);

public:
  NetSocket(DOMAIN, TYPE, PROTOCOL);

private:
  FD m_fd;
};

class SocketAddress {
public:
  virtual size_t Size() const = 0;
  virtual std::string Ip() const = 0;
  virtual in_port_t Port() const = 0;
  virtual DOMAIN Family() const = 0;
};

class SocketAddressV4 : public SocketAddress{
public:
  SocketAddressV4(in_port_t port, uint32_t ip) {
    memset(&m_addr, 0, Size());
    m_addr.sin_port = port;
    m_addr.sin_addr.s_addr = ip;
    m_addr.sin_family = AF_INET;

    char buf[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(m_addr.sin_addr), buf,INET_ADDRSTRLEN) == nullptr) {
      throw NetSocketException(errno);
    }
    m_ip = buf;
  }

  SocketAddressV4(in_port_t port, std::string ip)
    :m_ip(ip)
  {
    memset(&m_addr, 0, Size());
    m_addr.sin_port = port;
    m_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip.c_str(), &(m_addr.sin_addr)) != 1) {
      if (errno != 0) {
        throw NetSocketException(errno);
      } else {
        std::string str("inet_pton: ip is not a valid "
                        "network address");
        throw NetSocketException(str);
      }
    }
  }

  SocketAddressV4(const SOCKET_ADDRESS *address) {
    //TODO: add exeception handle
    memcpy(&m_addr, address, sizeof(SOCKET_ADDRESS_V4));

    char buf[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(m_addr.sin_addr), buf,INET_ADDRSTRLEN) == nullptr) {
      throw NetSocketException(errno);
    }
    m_ip = buf;
  }

  size_t Size() const {
    return sizeof(m_addr);
  }

  const SOCKET_ADDRESS_V4 *Get() const {
    return &m_addr;
  }

  std::string Ip() const {
    return m_ip;
  }

  in_port_t Port() const {
    return m_addr.sin_port;
  }

  DOMAIN Family() const {
    return m_addr.sin_family;
  }

private:
  SOCKET_ADDRESS_V4 m_addr;
  std::string m_ip;
};

#endif /* NET_SOCKET_H */
