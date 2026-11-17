// this code sends the response of the requests of a http server
#include <iostream>
#include <sstream>

#include <string>
#include <sys/socket.h>
#include <unordered_map>
class Response_http {
public:
  int statuscode;
  std::string StatusText;
  std::string responsebody;
  std::unordered_map<std::string, std::string> httpheaders;
  int socket;

  Response_http(int clientSocket, int httpCode, std::string Status,
                std::string body,
                std::unordered_map<std::string, std::string> headers) {
    statuscode = httpCode;
    StatusText = Status;
    responsebody = body;
    httpheaders = headers;
    socket = clientSocket;
  };
  // constructs the http response
  std::string serialize() {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << statuscode << " " << StatusText << "\r\n";
    for (const auto &[key, value] : httpheaders) {
      oss << key << ": " << value << "\r\n";
    }
    oss << "Content-Length: " << responsebody.size() << "\r\n";
    oss << "\r\n" << responsebody;
    return oss.str();
  };
  // send the http response
  bool send(int socket, const std::string &data) {
    size_t total_sent = 0;
    size_t bytes_left = data.size();
    const char *buffer = data.data();
    while (total_sent < data.size()) {
      ssize_t sent = ::send(socket, buffer + total_sent, bytes_left, 0);
      if (sent < 0) {
        std::cerr << "connection failed";
        return false;
      }
      total_sent += sent;
      bytes_left -= sent;
    }
    return true;
  }
};
