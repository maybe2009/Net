#ifndef NET_SOCKET_INCLUDED
#define NET_SOCKET_INCLUDED value

#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>

class NetSocket
{
public:
  typedef int FD_TYPE;
  typedef int DOMAIN_TYPE;
  typedef int TYPE_TYPE;
  typedef int PROTOCOL_TYPE;
  typedef struct sockaddr SOCK_ADDR_TYPE;
  typedef socklen_t SOCK_LEN_TYPE;
  typedef int FLAG;
private:
  FD_TYPE socket(DOMAIN_TYPE, FD_TYPE, PROTOCOL_TYPE);  
public:
  void  Bind(const SOCK_ADDR_TYPE* addr, SOCK_LEN_TYPE len);
  int   Connect(const SOCK_ADDR_TYPE* ADDR, SOCK_LEN_TYPE len);
  int   Accept(SOCK_ADDR_TYPE*, SOCK_LEN_TYPE*);
public:
  NetSocket(DOMAIN_TYPE, FD_TYPE, PROTOCOL_TYPE);
  virtual ~NetSocket();
  

  FD_TYPE m_fd;
};

#endif /* NET_SOCKET_H */

#endif /* ifndef NET_SOCKET_INCLUDED */
