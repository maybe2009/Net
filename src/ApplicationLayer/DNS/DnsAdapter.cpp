//
// Created by DW on 2016/8/12.
//

#include "DnsAdapter.h"

DnsAdapter::DnsAdapter(DOMIAN family,
                       TYPE type,
                       PROTOCOL protocol,
                       AI_FLAG flags)
{
  memset(&m_hint, 0, sizeof(m_hint));
  m_hint.ai_flags = flags
  m_hint.ai_family = family;
  m_hint.ai_socktype =type;
  m_hint.ai_protocol = protocol;
}

Meta DnsAdapter::Get(NodeName node, ServiceName service) {
  Meta result;
  int ret = getaddrinfo(node.c_str(), service.c_str(), &m_hint, &result);
  if (ret) {
    std::string errstr = gai_strerror(ret);
    throw NetSocketException(errstr);
  }
  return result;
}
