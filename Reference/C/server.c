/**
 * @file   server.c
 * @author Chinmay Kamat <chinmaykamat@cmu.edu>
 * @date   Fri, 15 February 2013 04:59:59 EST
 *
 * @brief A simple echo server
 *
 */

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/* Includes related to socket programming */
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

/* Local includes from ./inc */
#include <log.h>
#include <helper.h>
#include <config.h>

#define ARGS_NUM 1
#define MAX_LINE 128

int main(int argc, char **argv)
{
    int port;
    int serv_sock, client_sock;
    int optval = 1;
    int bytes_received, bytes_sent, total_sent;

    socklen_t len;
    struct sockaddr_in addr, client_addr;

    char *client_addr_string;
    char buffer[MAX_LINE];

    /*
     * ignore SIGPIPE, will be handled
     * by return value
     */
    signal(SIGPIPE, SIG_IGN);

    if (argc != (ARGS_NUM + 1)) {
        error_log("%s","Incorrect arguments provided\n"
                  "usage: ./server <port>");

        exit(EXIT_FAILURE);
    }

    client_addr_string = (char *) malloc(INET_ADDRSTRLEN);
    if (NULL == client_addr_string) {
        error_log("Unable to allocate memory for client_addr due to malloc() "
                  "error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Parse the port */
    port = atoi(argv[1]);
    if ((port > MAX_PORT) || (port < MIN_PORT)) {
        error_log("%s", "Port must be in range 1024 to 65535");
        exit(EXIT_FAILURE);
    }

    /* Create a socket for listening */
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error_log("socket() error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port);

    /*
     * bind() the socket to the ip address and the port. This actually
     * created the mapping between the socket and the IP:Port pair
     */
    if (bind(serv_sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        error_log("bind() error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval, sizeof(int)) < 0) {
        error_log("setsockopt() error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*
     * listen() for incoming connections.
     * The server accepts SYN packets after this call succeeds. If a client
     * calls connect() before this, the OS kernel will response with a RST and
     * terminate the connection. The BACKLOG is the total amount of connections
     * that can be in ESTABLISHED state (SYN-SYN/ACK-ACK complete at the server
     * before accept() is called.
     */
    if (listen(serv_sock, BACKLOG) < 0) {
        error_log("listen() error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    debug_log("Now listening on port %d", port);

    while(1) {
        total_sent = bytes_received = bytes_sent = 0;
        /* Accept the client connection  */
        len = sizeof(client_addr);

        /* Get a socket to actually communicate withe client
         * The client_sock returned by accept() is used for further
         * communication with the client while the serv_sock is still used for
         * new connections. accept() blocks if no connections are present
         */
        client_sock = accept(serv_sock,
                             (struct sockaddr *) &client_addr, &len);
        if (client_sock < 0) {
            error_log("Unable to add client due to accept() "
                      "error: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        inet_ntop(AF_INET, &(client_addr.sin_addr),
                  client_addr_string, INET_ADDRSTRLEN);
        debug_log("Accepted connection from %s:%d",
                  client_addr_string, ntohs(client_addr.sin_port));

        /* Read the date sent from the client */
        bytes_received = recv(client_sock, buffer, MAX_LINE - 1, 0);

        /*
         * Echo - Write (send) the data back to the client taking care of short
         * counts
         */
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Read from client %s:%d -- %s\n",
                   client_addr_string, ntohs(client_addr.sin_port), buffer);
            while (total_sent != bytes_received) {
                bytes_sent = send(client_sock, buffer + total_sent,
                                  bytes_received - total_sent, 0);
                if (bytes_sent <= 0) {
                    break;
                } else {
                    total_sent += bytes_sent;
                }
            }
        }
        debug_log("Closing connection %s:%d",
                  client_addr_string, ntohs(client_addr.sin_port));
        /* Our work here is done. Close the connection to the client */
        close(client_sock);
    }

    return 0;
}
