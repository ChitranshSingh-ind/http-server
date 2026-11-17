
#include <cstddef>

#include <map>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

struct httprequest {
  int clientSocket = -1;
  std::string method;  // e.g., "GET", "POST"
  std::string path;    // e.g., "/index.html"
  std::string version; // e.g., "HTTP/1.1"
  std::map<std::string, std::string> headers;
  std::string body;
};
class incoming {
public:
  std::string port;
  int socketFd;

  incoming(std::string portinput, int socket_fd) {
    port = portinput;
    socketFd = socket_fd;
  };
  httprequest scan() {
    while (1) {
      int clientSocket = accept(socketFd, nullptr, nullptr);
      httprequest complete_request;
      if (clientSocket < 0) {
        continue;
      }
      char buffer[4096];
      auto bytes_read = read(clientSocket, buffer, sizeof(buffer) - 1);
      if (bytes_read > 0) {
        std::string raw_request(buffer, bytes_read);
        std::string first_line =
            raw_request.substr(0, raw_request.find("/r/n"));
        complete_request = parse(raw_request);
        complete_request.clientSocket = clientSocket;
      } else {
        close(clientSocket);
      }
      return complete_request;
    }
  };

private:
  httprequest parse(const std::string &raw) {
    httprequest rq;
    size_t headerend = raw.find("\r\n");
    std::string header = raw.substr(0, headerend);
    if (headerend != std::string::npos) { // if the headers are not the end then
                                          // extract the body
      rq.body = raw.substr(headerend + 4);
    }
    std::istringstream stream(header);
    std::string first_line;
    if (std::getline(stream, first_line)) {
      if (!first_line.empty() &&
          first_line.back() == '\r') { // removes the last return carridge
                                       // character from the first line
        first_line.pop_back();
      }
    }
    std::istringstream first_line_stream(first_line);
    first_line_stream >> rq.method >> rq.path >>
        rq.version; // seperates the method, path and version from the first
                    // line of request
    // now put remaining headers in the map in key value pairs
    while (std::getline(stream, first_line)) {
      if (!first_line.empty() && first_line.back() == '\r') {
        first_line.pop_back(); // remove the \r from end
      }
      // defining colon as the place to seperate the key from value
      size_t colon = first_line.find(':');
      if (colon != std::string::npos) {
        std::string key = first_line.substr(
            0, colon); // seperating and storing the key into a string
        std::string value =
            first_line.substr(colon + 1); // seperating and storing the value of
                                          // header into the value string

        size_t firstChar = value.find_first_not_of(" ");
        if (firstChar != std::string::npos) {
          value = value.substr(firstChar);
        }
        rq.headers[key] = value;
      }
    }
    return rq;
  };
};
