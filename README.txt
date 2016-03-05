# README.txt - 14-740 Lab 2 "Simple : An HTTP 1.0 Web Server"
# Author : Chinmay Kamat <chinmaykamat@cmu.edu>
# Last Modified : Fri, 15 February 2013 05:25:08 EST

* What's included in here :
** docs/
   This folder contains the Annotated RFC with hints for
   Simple implementation
** src/
   The support code

* Support Code :
** Files
   - server.c / Server.java : A simple TCP echo server
   - client.c / Client.java : A simple TCP client
   - getmime.c/ GetMime.java: A utility to get MIME type to be used in Content-Type header
   - log.h : A logging infrastructure that you can use

** Compilation
   Both the C and Java folders contain Makefile. Just run make to get executables. C binaries
   can be run as

   ./server

   and Java .class files can be run as

   java Server

   The programs will print usage instructions if you do not pass correct parameters.

** Reference implementation of Simple
   This is included in the reference/ directory.
   To run this simply run the following

   ./simple.py 8080 $PWD/www

   this is server the www directory in the reference directory at port 8080 on localhost.
   The responses sent by your implementation of Simple must match the responses sent by this
   reference implementation.
