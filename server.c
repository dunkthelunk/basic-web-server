#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT "8863"
#define BACKLOG 10

int main() {
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, &hints, &res);
  int socket_fd, newfd; 
  socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  bind(socket_fd, res->ai_addr, res->ai_addrlen);

  void *addr;
  unsigned short int port;
  char ipstr[INET6_ADDRSTRLEN];
  struct sockaddr_in *address = (struct sockaddr_in *)res->ai_addr;
  addr = &(address->sin_addr);
  port = address->sin_port;
  inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr);
  port = ntohs(port);
  printf("Listening at %s on port %i\n", ipstr, port);

  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  listen(socket_fd, BACKLOG);
  char *msg1= "Jeez was here once!";
  char *msg2= "Jeez was here twice!";
  int bytes_sent;
  char recvbuf[1000];
  while(1) {
    addr_size = sizeof their_addr;
    newfd = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);
    recv(newfd, &recvbuf, 1000, 0);
    printf("req received %s\n", recvbuf);
    if(recvbuf[0] == '1') {
      bytes_sent = send(newfd, msg1, strlen(msg1), 0); 
    } else if(recvbuf[0] == '2') {
      bytes_sent = send(newfd, msg2, strlen(msg2), 0); 
    }
    close(newfd);
    printf("closed %i\n", newfd);
  }
  freeaddrinfo(res);
  close(newfd);
  return 0;
}
