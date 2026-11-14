#include <cstring>  //for c style string operations needed in few parts
#include <iostream> //for standard io
#include <netinet/in.h> //Defines the sockaddr_in structure and internet address constants
#include <sstream>      //for http parsing
#include <string>       //for using string instead of c style char*
#include <sys/socket.h> //for functions of socket
#include <unistd.h>     //for open, write, close etc.
// this is the code to make a http server socket, it would
// listen for incoming requests and output whatever it is requested, provided it
// exists

// the server would check the url requested and try to obtain that file by
// requesting it in the folder, our server would have many vulnerabilities but
// thats okay for now at first I would only do static pages and than extend it
// to more advanced capabilities
//
// we need to parse the requests to obtain the following things
// Method
// requested resource
// currently I would try to only do this and then we would make it more advanced

// sent response must resemble this
// HTTP/1.1 200 OK
// Date: Fri, 01 Jul 2022 12:00:00 GMT
// Server: Apache/2.4.41 (Ubuntu)
// Last-Modified: Mon, 13 Jun 2022 10:00:00 GMT
// Content-Length: 1234
// Content-Type: text/html; charset=UTF-8
// Connection: keep-alive
//
// rest of the response body
// A log will also be maintained in a file named log.Md
// it would log the ip, user agent, time and url of requested pages
//

class server {
public:
  // our port where the server will listen
  std::string port;
  // constructor for defining out port
  server(std::string port) { this->port = port; };
  int start() {
    int sock_fd;                    // file descriptor for server socket
    struct sockaddr_in server_addr; // this is the structure we would use to
                                    // store our server's address
    sock_fd = socket(
        AF_INET, SOCK_STREAM.0); // we will use ipv4, used SOCK_STREAM for TCP,
                                 // also used 0 in the protocol field so that
                                 // best protocol is automatically chosen
    //  On success, a file descriptor for the new socket is returned.  On error,
    //  -1 is returned, and errno is set to indicate the error.
    // checking for any error
    if (sock_fd < 0) {
      std::cerr << "Failed to initialise the socket";
      exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = (std::stoi(port));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
      std::cerr << "Can't bind address";
      exit(1);
    }
    if (listen(sock_fd, 5) < 0) {
      std::cerr << "Can't listen on socket";
      exit(1);
    }
    std::cout << "Listening on port" << port << endl;
  };
  int close() { return 0; }
};

int main() {}
