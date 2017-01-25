CC = gcc
CFLAGS = -Og
LDLIBS = -lpthread

PROGS = echo-client echo-server

all: $(PROGS)

echo-client: echo-client.c csapp.c
echo-server: echo-server.c csapp.c


clean:
	rm -rf *~ $(PROGS)
