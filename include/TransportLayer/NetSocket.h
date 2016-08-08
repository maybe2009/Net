#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <exception>
#include <cstring>

#define MAX_SIZE 256

class NetSocket
{
public:
  typedef int FD;
  typedef int DOMAIN;
  typedef int TYPE;
  typedef int PROTOCOL;
  typedef struct sockaddr ADDR;
  typedef socklen_t SOCK_LEN_TYPE;
  typedef int FLAG;

private:
  void CreateSocket(DOMAIN, TYPE, PROTOCOL);

public:
  void  Bind(const ADDR* addr, SOCK_LEN_TYPE len);
  void  Listen(int backlog);
  int   Connect(const ADDR* addr, SOCK_LEN_TYPE len);
  int   Accept(ADDR*, SOCK_LEN_TYPE*);

public:
  NetSocket(DOMAIN, FD, PROTOCOL);
  virtual ~NetSocket();

private:
  FD m_fd;
};

class NetSocketException : public std::exception {
public:
  NetSocketException(int errno);
  virtual const char* what() const override ;

private:
  int m_errno;
  char m_errstr[MAX_SIZE];
};
#endif /* NET_SOCKET_H */
