//
// Created by DW on 2016/8/19.
//

#ifndef NET_NETEXECEPTIONS_H
#define NET_NETEXECEPTIONS_H

#include <exception>
#include <cstring>
#include <string>

#define MAX_SIZE 256

class NetSocketException : public std::exception {
public:
  explicit NetSocketException(int v)
      : m_errno(v)
  {
    //use GNU version strerror_r
    m_errstr = strerror_r(m_errno, m_errstr_buf, MAX_SIZE);
    //for safety, ensure m_errstr_buf has at least one null-terminator
    m_errstr_buf[MAX_SIZE] = 0;
  }

  explicit NetSocketException(std::string& str) {
    m_errstr = str;
  }

  virtual const char* what() const noexcept override {
    return m_errstr.c_str();
  }

  int ErrorNumber() const {
    return m_errno;
  }

private:
  int m_errno;
  char m_errstr_buf[MAX_SIZE + 1];
  std::string m_errstr;
};

#endif //NET_NETEXECEPTIONS_H
