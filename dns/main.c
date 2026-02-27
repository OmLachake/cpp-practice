#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    struct addrinfo req, *res;

    memset(&req,0,sizeof(req));
    req.ai_family = AF_UNSPEC;
    req.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo("example.com", "80", &req, &res);
    if( status != 0 )
    {
        printf("DNS Lookup Failed : %s\n",gai_strerror(status));
        return 1;
    }

    printf("DNS Lookup succesful!\n");
    printf("Printng the addresses now : !\n");

    char ipstr[INET6_ADDRSTRLEN];
    struct addrinfo *p;

    for(p=res;p!=NULL;p=p->ai_next)
    {
        void *addr;
        char *ipver;

        // Check IP version : 

        if(p->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPV4";
        }else if (p->ai_family == AF_INET6)
        {
            struct sockaddr_in *ipv6 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv6->sin_addr);
            ipver = "IPV6";
        }


        // Convert the raw bytes into human readable string
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));

        printf("\n%s : %s",ipver,ipstr);
    }

    p = res;
    int clientSocket = socket(p->ai_family,p->ai_socktype,p->ai_protocol);

    status = connect(clientSocket,p->ai_addr,p->ai_addrlen);

    if(status == -1)
    {
        printf("Failed to Connect the Dial!");
    }
    else {
        printf("\nConnected to example.com\n");
    }


    char *request = 
                    "GET / HTTP/1.1\r\n"
                    "Host: example.com\r\n"
                    "Connection: close\r\n"
                    "\r\n";
    send(clientSocket,request,strlen(request),0);

    char buffer[32768];
    int bytesReceived;
    while((bytesReceived = recv(clientSocket, buffer, sizeof(buffer)-1, 0)) > 1){
        buffer[bytesReceived]='\0';
        printf("%s",buffer);
    }

    close(clientSocket);

    freeaddrinfo(res);
    return 0;
}