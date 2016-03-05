CC      = gcc
CFLAGS  = -Wall -Werror -I ./inc

default: getmime server client

.PHONY: clean

getmime: getmime.c helper.c
server: server.c
client: client.c

debug: CFLAGS = -g -Wall -Werror -DDEBUG -DLOG_LEVEL=2 -I ./inc
debug: getmime server client

clean:
	rm -f *.o getmime server client
