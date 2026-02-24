#include <arpa/inet.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

using json = nlohmann::json;
using std::string, std::vector, std::cout;

json getBasicRequestInfo(char buffer[]) {
  string line;
  std::stringstream ss(buffer);
  std::getline(ss, line);

  size_t pos = 0;
  pos = line.find(" ");
  string method = line.substr(0, pos);

  line.erase(0, pos + strlen(" "));
  pos = line.find(" ");
  string url = line.substr(0, pos);

  json jsonReq;
  jsonReq["method"] = method;
  jsonReq["url"] = url;

  return jsonReq;
}

int main() {
  int my_socket = socket(AF_INET, SOCK_STREAM, 0);

  int opt = 1;
  if (setsockopt(my_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt failed");
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int bindResult = bind(my_socket, (struct sockaddr *)&server_address,
                        sizeof(server_address));
  if (bindResult) {
    perror("Bind Failed");
  }

  listen(my_socket, 5);
  cout << "Server listening on port 8080...\n";

  while (1) {
    int client_socket = accept(my_socket, NULL, NULL);
    cout << "A client Connected\n";

    char buffer[1024] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);

    json request = getBasicRequestInfo(buffer);
    string method, url;
    method = request.value("method", "GET");
    url = request.value("url", "/");

    cout << "Request Type   : " << method << "\nMethod      : " << url << "\n";

    string message;

    string mainHTMLResponse =
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n\n"
        "<html><body><h1>HI OM</h1></body></html>";

    string pageNotFound =
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n\n"
        "<html><body><h1>PAGE NOT FOUND. ERROR 404</h1></body></html>";

    if (method == "GET") {
      if (url == "/") {
        message = mainHTMLResponse;
      } else {
        message = pageNotFound;
      }
    } else {
      message = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n\nNot a "
                "correct METHOD.";
    }

    send(client_socket, message.c_str(), message.length(), 0);

    cout << "Message Sent. Closing now. Waiting for the next client...\n\n";
    close(client_socket);
  }

  close(my_socket);
  return 0;
}
