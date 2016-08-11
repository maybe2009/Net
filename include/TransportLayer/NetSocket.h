#ifndef NET_SOCKET_H
#define NET_SOCKET_H

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
typedef struct sockaddr ADDR;
typedef socklen_t SOCK_LEN_TYPE;
typedef int FLAG;

class NetSocket
{
private:
  void CreateSocket(DOMAIN, TYPE, PROTOCOL);

public:
  void      Bind(const ADDR* addr, SOCK_LEN_TYPE len);
  void      Listen(int backlog);
  int       Connect(const ADDR* addr, SOCK_LEN_TYPE len);
  int       Accept(ADDR*, SOCK_LEN_TYPE*);
  ssize_t   Read(void *buf, size_t count);
  ssize_t   Write(const void *buf, size_t count);

public:
  NetSocket(DOMAIN, TYPE, PROTOCOL);

private:
  FD m_fd;
};

class NetSocketException : public std::exception {
public:
  explicit NetSocketException(int);
  explicit NetSocketException(std::string&);
  virtual const char* what() const noexcept override ;

private:
  int m_errno;
  char m_errstr_buf[MAX_SIZE + 1];
  std::string m_errstr;
};

class SocketAddressV4 {
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
    :m_ip(ip) {
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

  size_t Size() const {
    return sizeof(m_addr);
  }

  const struct sockaddr_in* Get() const {
    return &m_addr;
  }

  std::string Ip() const {
    return m_ip;
  }

  in_port_t Port() const {
    return m_addr.sin_port;
  }

private:
  struct sockaddr_in m_addr;
  std::string m_ip;
};

#endif /* NET_SOCKET_H */
