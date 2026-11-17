#include "request.cpp"
#include "response.cpp"
#include "server.cpp"
#include <fstream>
#include <iostream>

#include <string>
#include <unordered_map>
int main() {
  std::string port = "8080";
  server mainserver(port);
  int serverstatus = mainserver.start();
  if (!(serverstatus == 0)) {
    std::cerr << "couldn't start server";
    return 1;
  }
  std::unordered_map<std::string, std::string> file404head = {
      {"content-type", "text/html"}

  }; // currently adding static headers to the header maps

  std::unordered_map<std::string, std::string> filehead = {
      {"content-type", "text/html"}};
  while (1) {
    std::ifstream file("index.html");
    std::ifstream file404("file404.html");
    int socket_fd = mainserver.sock_fd;
    incoming remoteRequest(port, socket_fd);
    httprequest request = remoteRequest.scan();
    if (request.method == "GET") {
      if (request.path == "/") {
        if (!file.is_open()) {
          std::cerr << "couldn't open file";
          std::ostringstream buffer;
          buffer << file404.rdbuf();
          Response_http rtoc(request.clientSocket, 404, "Not Found",
                             buffer.str(), file404head);
          rtoc.send(request.clientSocket, rtoc.serialize());

          close(request.clientSocket);

        } else {
          std::ostringstream buffer;
          buffer << file.rdbuf();
          Response_http rtoc(request.clientSocket, 200, "OK", buffer.str(),
                             filehead);
          rtoc.send(request.clientSocket, rtoc.serialize());

          close(request.clientSocket);
        }
      } else {
        std::ostringstream buffer;
        buffer << file404.rdbuf();
        std::cerr << "couldn't find file";

        Response_http rtoc(request.clientSocket, 404, "Not Found", buffer.str(),
                           file404head);
        rtoc.send(request.clientSocket, rtoc.serialize());

        close(request.clientSocket);
      }
    }
  }
}
