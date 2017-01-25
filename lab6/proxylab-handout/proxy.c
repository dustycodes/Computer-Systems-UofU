/*
 * proxy.c - CS:APP Web proxy
 *
 * IMPORTANT: Give a high level description of your code here. You
 * must also provide a header comment at the beginning of each
 * function that describes what that function does.
 */

#include "csapp.h"
/*
 * Globals
 */
#define PROXY_LOG "proxy.log"  // proxy log file
FILE *log_file; /* Log file with one line per HTTP request */
sem_t s_log;

/*
 * Function prototypes
 */
void *process_request(void *args);
int parse_uri(char *uri, char *target_addr, char *path, int  *port);
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);
void close_fd(int fd1, int fd2);

/*
 * structure to keep track of threads for the multi-threaded implementation
 */
struct thread_keeper {
  struct sockaddr_in socket;
  int connection;
};

/*
 * main - Main routine for the proxy program
 */
int main(int argc, char **argv)
{
    int port;
    int connfd;
    struct sockaddr_in clientaddr;

    /* Check arguments */
    if (argc != 2) {
	     fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
	     exit(0);
    }

    /* Ignore SIGPIPE signals */
    Signal(SIGPIPE, SIG_IGN);

    /* Initialize */
    Sem_init(&s_log, 0, 1);
    log_file = Fopen(PROXY_LOG, "a");
    if(log_file == NULL) {
      printf("There needs to be a log file %s", PROXY_LOG);
      exit(1);
    }
    port = atoi(argv[1]); // what is the port number?
    int listenfd = Open_listenfd(port);
    pthread_t thd;
    socklen_t clientlen;
    int memsize = sizeof(struct thread_keeper);

    /* Wait for and process client connections */
    while (1) {
      clientlen = (socklen_t) sizeof(clientaddr);
      /* for debugging */
      printf("server is running on port %d \n", port);
      P(&s_log);
      struct thread_keeper *temp = (struct thread_keeper *) malloc(memsize);
      connfd = Accept(listenfd, (SA *)(&(temp->socket)), &clientlen);
      temp->connection = connfd;
      Pthread_create(&thd, NULL, process_request, (void *)temp);
      V(&s_log);

       /*  Dont need to do this, already executed in thread */
       /* sequential call to process_request
      process_request(connfd, clientaddr); */
    }

    exit(0);
}



/*
 * process_request - Executed sequentially or by each thread
 *
 * Read an HTTP request from a client, forward it to the
 * end server (always as a simple HTTP/1.0 request), wait for the
 * response, and then forward back to the client.
 *
 */
void *process_request(void *args)
{
  /* translate the arguments from the argument passing
  structure thread_keeper. Memory was allocated so it will
  need to be freed */
    Pthread_detach(pthread_self());
    struct thread_keeper *keeper = (struct thread_keeper *)args;
    struct sockaddr_in clientaddr;
    int connfd = keeper->connection;
    memcpy(&clientaddr, &(keeper->socket), sizeof(struct sockaddr_in));
    free(args);


    int realloc_factor;             /* Used to increase size of request buffer if necessary */
    char *request;                  /* HTTP request from client */
    char *request_uri;              /* Start of URI in first HTTP request header line */
    char *request_uri_end;          /* End of URI in first HTTP request header line */
    char *rest_of_request;          /* Beginning of second HTTP request header line */
    char buf[MAXLINE];              /* General I/O buffer */
    int request_len;                /* Total size of HTTP request */
    int i, n;                       /* General index and counting variables */
    rio_t  rio;

    /*
     * Read the entire HTTP request into the request buffer, one line
     * at a time.
     */
    request = (char *)Malloc(MAXLINE);
    request[0] = '\0';
    realloc_factor = 2;
    request_len = 0;
    Rio_readinitb(&rio, connfd);
    while (1) {
	if ((n = Rio_readlineb(&rio, buf, MAXLINE)) <= 0) {
	    printf("process_request: client issued a bad request (1).\n");
	    close(connfd);
	    free(request);
	    return NULL;
	}

	/* If not enough room in request buffer, make more room */
	if (request_len + n + 1 > MAXLINE)
	    Realloc(request, MAXLINE*realloc_factor++);

	strcat(request, buf);
	request_len += n;

	/* An HTTP requests is always terminated by a blank line */
	if (strcmp(buf, "\r\n") == 0)
	    break;
    }

    /*
     * Make sure that this is indeed a GET request
     */
    if (strncmp(request, "GET ", strlen("GET "))) {
	printf("process_request: Received non-GET request\n");
	close(connfd);
	free(request);
	return NULL;
    }
    request_uri = request + 4;

    /*
     * Extract the URI from the request
     */
    request_uri_end = NULL;
    for (i = 0; i < request_len; i++) {
	if (request_uri[i] == ' ') {
	    request_uri[i] = '\0';
	    request_uri_end = &request_uri[i];
	    break;
	}
    }

    /*
     * If we hit the end of the request without encountering a
     * terminating blank, then there is something screwy with the
     * request
     */
    if ( i == request_len ) {
	printf("process_request: Couldn't find the end of the URI\n");
	close(connfd);
	free(request);
	return NULL;
    }

    /*
     * Make sure that the HTTP version field follows the URI
     */
    if (strncmp(request_uri_end + 1, "HTTP/1.0\r\n", strlen("HTTP/1.0\r\n")) &&
	strncmp(request_uri_end + 1, "HTTP/1.1\r\n", strlen("HTTP/1.1\r\n"))) {
	printf("process_request: client issued a bad request (4).\n");
	close(connfd);
	free(request);
	return NULL;
    }

    /*
     * We'll be forwarding the remaining lines in the request
     * to the end server without modification
     */
    rest_of_request = request_uri_end + strlen("HTTP/1.0\r\n") + 1;

    printf("request_uri %s\n", request_uri);
    printf("rest_of_request %s\n", rest_of_request);



  //TODO: Call parse_uri, URI is in request_uri, to be used as input to parse_uri
  char hostname[MAXLINE];
  char pathname[MAXLINE];
  char l[MAXLINE];
  int port;
  if(parse_uri(request_uri, hostname, pathname, &port)) {
    fprintf(stderr, "error in parse_uri\n");
    /*TODO: add error message for client */
    if(connfd > 0) {
      Close(connfd);
    }
    return NULL;
  }
  printf("good request\n");
  printf("pathname %s\n", pathname);
  printf("hostname %s\n", hostname);
  printf("port: %d\n", port);
  sprintf(l, "%s %s %s\r\n%s", "GET /", pathname, "HTTP/1.0", rest_of_request);

  rio_t rio_client;
  int clientfd = open_clientfd(hostname, port);
  Rio_readinitb(&rio_client, clientfd);
  Rio_writen(clientfd, l, strlen(l));
  printf("header from client: %s\n", l);

  /* for if you want to add to the request*/
  /*while(Rio_readlineb(&rio, l, MAXLINE) > 2) {
    Rio_writen(clientfd, l, strlen(l));
  }
  Rio_writen(clientfd, "\r\n", 4);*/
  ssize_t tempsize;
  int mysize=0;

  while((tempsize=Rio_readlineb(&rio_client, buf, MAXLINE)) > 2) {
    printf("header read from server: %s", buf);
    Rio_writen(connfd, buf, strlen(buf));
    mysize = mysize + (int)tempsize;
    printf("mysize: %d\n", mysize);
  }
  Rio_writen(connfd, "\r\n", 2);

  char body[MAXLINE];
  while((tempsize = Rio_readnb(&rio_client, body, MAXLINE)) > 0) {
    Rio_writen(connfd, body, (int)tempsize);
    mysize = mysize + (int)tempsize;
  }

  // GET http://www.utah.edu/ HTTP/1.0
  printf("ready to write log\n");
  //printf("buffer: %s\n", buf);
  printf("request_uri; %s\n", request_uri);
  printf("total_size: %d\n", mysize);

  //void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size)
  char logString[MAXLINE];
  format_log_entry(logString, &clientaddr, request_uri, mysize);
  printf("write to log: %s\n ", logString);

  fprintf(log_file, logString);
  fflush(log_file);
  close_fd(clientfd, connfd);

    return NULL;
}

void close_fd(int fd1, int fd2) {
  if(fd1 > 0)
    Close(fd1);
  if(fd2 > 0)
    Close(fd2);
}

/*
 * parse_uri - URI parser
 *
 * Given a URI from an HTTP proxy GET request (i.e., a URL), extract
 * the host name, path name, and port.  The memory for hostname and
 * pathname must already be allocated and should be at least MAXLINE
 * bytes. Return -1 if there are any problems.
 */
int parse_uri(char *uri, char *hostname, char *pathname, int *port)
{
    char *hostbegin;
    char *hostend;
    char *pathbegin;
    int len;

    if (strncasecmp(uri, "http://", 7) != 0) {
	hostname[0] = '\0';
	return -1;
    }

    /* Extract the host name */
    hostbegin = uri + 7;
    hostend = strpbrk(hostbegin, " :/\r\n\0");
    len = hostend - hostbegin;
    strncpy(hostname, hostbegin, len);
    hostname[len] = '\0';

    /* Extract the port number */
    *port = 80; /* default */
    if (*hostend == ':')
	*port = atoi(hostend + 1);

    /* Extract the path */
    pathbegin = strchr(hostbegin, '/');
    if (pathbegin == NULL) {
	pathname[0] = '\0';
    }
    else {
	pathbegin++;
	strcpy(pathname, pathbegin);
    }

    return 0;
}

/*
 * format_log_entry - Create a formatted log entry in logstring.
 *
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), and the size in bytes
 * of the response from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr,
		      char *uri, int size)
{
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /*
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 13, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;


    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s %d\n", time_str, a, b, c, d, uri, size);
}
