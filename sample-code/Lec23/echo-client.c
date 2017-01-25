#include "csapp.h"

int main(int argc, char **argv)
{
  int clientfd;
  char *host, *port, buf[MAXLINE], buf2[MAXLINE];
  rio_t rio;

  host = argv[1];
  port = argv[2];

  clientfd = Open_clientfd(host, port);
  Rio_readinitb(&rio, clientfd);

  while (Fgets(buf, MAXLINE, stdin) != NULL) {
    Rio_writen(clientfd, buf, strlen(buf));
    Rio_readlineb(&rio, buf2, MAXLINE);
    Fputs(buf2, stdout);
  }
  Close(clientfd); 
  exit(0);
}

