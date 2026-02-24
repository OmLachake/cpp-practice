#include <arpa/inet.h>
#include <iostream>
#include <libgen.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using json = nlohmann::json;
using std::string, std::cout;

string readFile(string fileName) {
  // 1. Open file and check if it exists
  FILE *file = std::fopen(fileName.c_str(), "rb");
  if (!file) {
    perror("File open failed");
    return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
  }

  // 2. Get file size
  std::fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  // 3. Prepare the HTTP header string first
  string header = "HTTP/1.1 200 OK\r\n"
                  "Content-Type: image/webp\r\n"
                  "Content-Length: " +
                  std::to_string(fileSize) +
                  "\r\n"
                  "Connection: close\r\n"
                  "\r\n";

  // 4. Create a string that contains the header
  string message = header;

  // 5. Resize message to hold header + file data
  size_t headerSize = message.size();
  message.resize(headerSize + fileSize);

  // 6. Read the file data directly into the string's buffer
  // &message[headerSize] points to the spot immediately after the headers
  std::fread(&message[headerSize], 1, fileSize, file);

  std::fclose(file);
  return message;
}

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

int main(int argc, char *argv[]) {
  char *path_to_binary = strdup(argv[0]);
  char *directory = dirname(path_to_binary);
  chdir(directory);

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
        "HTTP/1.1 404 OK\r\nContent-Type: text/html\r\n\r\n\n"
        "<html><body><h1>PAGE NOT FOUND. ERROR 404</h1></body></html>";

    string falseRequest =
        "HTTP/1.1 403 OK\r\nContent-Type: text/plain\r\n\r\n\n"
        "Access Denied";

    if (url == "/") {
      if (method == "GET") {
        message = mainHTMLResponse;
      } else {
        message = falseRequest;
      }
    } else if (url == "/monkey") {
      if (method == "GET") {
        message = readFile("./assets/monkey.webp");
      }
    } else {
      message = pageNotFound;
    }

    send(client_socket, message.c_str(), message.length(), 0);
    cout << "Message Sent. Closing now. Waiting for the next client...\n\n";
    close(client_socket);
  }

  close(my_socket);
  return 0;
}
