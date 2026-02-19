#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
  int my_socket=socket(AF_INET,SOCK_STREAM,0);

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  server_address.sin_addr.s_addr = INADDR_ANY;

  const int tmp = bind(my_socket,(struct sockaddr*)&server_address, sizeof(server_address));
  listen(my_socket,5);
  printf("server listening on port 8080...\n");

  while(1)
  {
    int client_socket = accept(my_socket,NULL,NULL);
    printf("A client Connected\n");

    char buffer[1024]={0};
    recv(client_socket,buffer,sizeof(buffer),0);

    printf("Browser Info : \n");
    printf("-------------- \n");
    printf("%s\n",buffer);
    printf("------------ : \n");

    char *message="HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n\nHellow from the server";
    send(client_socket,message,strlen(message),0);
    
    printf("Message Sent. Closing now. Waiting for the next client...\n\n");
    close(client_socket);
  }

  close(my_socket);
  return 0;
}

