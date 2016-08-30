//
// Created by DW on 2016/8/15.
//

#ifndef NET_HTTPREQUEST_H
#define NET_HTTPREQUEST_H

#include <string>
#include <map>

typedef std::string Request;
typedef std::string HeaderLine;
typedef std::string Header;
typedef std::string Body;
typedef std::string Url;
typedef std::string Method;
typedef std::map<std::string, std::string> HeaderMap;

class HttpRequest {
public:
  HttpRequest(std::string http_version) :
      m_http_version(http_version) {}
  HttpRequest():
      m_http_version(STR_HTTP_V1_1){}

  std::string HttpGet(Url &url, HeaderMap &map, Body &body) {
    return AssembleRequest("GET", url, map, body);
  }

private:
  std::string AssembleRequest(Method method, Url url, HeaderMap &map, Body body) {
    m_header_line = method + STR_SPACE + url + STR_SPACE + m_http_version + STR_CRLF;
    for (auto &&item : map) {
      m_header += item.first + STR_COLON + item.second + STR_CRLF;
    }
    m_request = m_header_line + m_header + body;
    return m_request;
  }

private:
  Request m_request;
  HeaderLine m_header_line;
  Header m_header;
  HeaderMap m_header_map;
  std::string m_http_version;
  static std::string STR_SPACE;
  static std::string STR_CRLF;
  static std::string STR_COLON;
  static std::string STR_HTTP_V1_1;
};

std::string HttpRequest::STR_SPACE= " ";
std::string HttpRequest::STR_CRLF = "\r\n";
std::string HttpRequest::STR_COLON = ": ";
std::string HttpRequest::STR_HTTP_V1_1 = "";

#endif //NET_HTTPREQUEST_H
