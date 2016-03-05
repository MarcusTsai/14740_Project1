/**
 * @file   client.c
 * @author Chinmay Kamat <chinmaykamat@cmu.edu>
 * @date   Fri, 15 February 2013 04:53:41 EST
 *
 * @brief A simple TCP client
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
#include <netdb.h>

/* Local includes from ./inc */
#include <log.h>
#include <helper.h>
#include <config.h>

#define ARGS_NUM 2
#define MAX_LINE 128

int main(int argc, char **argv)
{
    int port;
    int client_sock;
    int bytes_received;
    int status;

    struct addrinfo hints;
    struct addrinfo *servinfo;

    char buffer[MAX_LINE];

    /*
     * ignore SIGPIPE, will be handled
     * by return value
     */
    signal(SIGPIPE, SIG_IGN);

    /* Make sure the cmd line arguments are correct */
    if (argc != (ARGS_NUM + 1)) {
        error_log("%s","Incorrect arguments provided\n"
                  "usage: ./client <server_ip> <server_port>");

        exit(EXIT_FAILURE);
    }

    /* Parse the port */
    port = atoi(argv[2]);
    if ((port > MAX_PORT) || (port < MIN_PORT)) {
        error_log("%s", "Port must be in range 1024 to 65535");
        exit(EXIT_FAILURE);
    }

    /* Create the hints structure to get the servinfo socket structure */
    hints.ai_family = AF_UNSPEC;     /* Don't care if its IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP Connection */
    hints.ai_protocol = IPPROTO_TCP; /* TCP Protocol */
    hints.ai_flags = AI_PASSIVE;     /* fill in my IP for me */

    if ((status = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        error_log("getaddrinfo() error: %s", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    while (1) {
        /* Read input from user */
        fgets(buffer, MAX_LINE - 1, stdin);

        /* Create a socket to connect to the server */
        if((client_sock = socket(servinfo->ai_family,
                                 servinfo->ai_socktype,
                                 servinfo->ai_protocol)) < 0) {
            error_log("socket() failed : %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
        /*
         * connect() to the server:
         * This initiates the SYN-SYN/ACK-ACK handshake with the server. The
         * call is blocked till the connection is complete. Only succeeds if the
         * server has already called listen()
         *
         */
        if (connect(client_sock, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
            error_log("connect() failed : %s", strerror(errno));
            return EXIT_FAILURE;
        }

        /* Use the returned socket for futher communication with the server */
        send(client_sock, buffer, strlen(buffer), 0);

        /* Receive the echo from the server */
        if ((bytes_received = recv(client_sock, buffer, MAX_LINE - 1, 0)) > 1) {
            buffer[bytes_received] = '\0';
            printf("Received %s\n", buffer);
        }
        /* Close the socket */
        close(client_sock);
    }
    /* Free servinfo allocated by getaddrinfo() */
    freeaddrinfo(servinfo);
    return 0;
}
