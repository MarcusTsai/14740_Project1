
/**

 * @file: Server.java
 * 
 * @author: Chinmay Kamat <chinmaykamat@cmu.edu>
 * 
 * @date: Feb 15, 2013 1:13:37 AM EST
 * 
 */
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * This function is designer for listening
 * request from the client. And each thread
 * will be generated for each request from client.
 */

public class Server {
    private static ServerSocket srvSock;

    public static void main(String args[]) {
        int port = 8080;

        /* Parse parameter and do args checking */
        if (args.length < 1) {
            System.err.println("Usage: java Server <port_number>");
            System.exit(1);
        }

        try {
            port = Integer.parseInt(args[0]);
        } catch (Exception e) {
            System.err.println("Usage: java Server <port_number>");
            System.exit(1);
        }

        if (port > 65535 || port < 1024) {
            System.err.println("Port number must be in between 1024 and 65535");
            System.exit(1);
        }

        try {
            /*
             * Create a socket to accept() client connections. This combines
             * socket(), bind() and listen() into one call. Any connection
             * attempts before this are terminated with RST.
             */
            srvSock = new ServerSocket(port, 100);
            System.out.println("Simple Server is Waiting for client on port " + port);

        Socket clientSock;
        Runnable clientHandler;

        while (true) {
            try {
                /*
                 * Get a sock for further communication with the client. This
                 * socket is sure for this client. Further connections are still
                 * accepted on srvSock
                 */
                clientSock = srvSock.accept();
                System.out.println(
                        "Accpeted new connection from " + clientSock.getInetAddress() + ":" + clientSock.getPort());
                clientHandler = new ClientHandler(clientSock);

                // Generate a thread
                Thread handleThread = new Thread(clientHandler);
                handleThread.start();

            } catch (IOException e) {

                System.out.println("Accept failed: " + port);
                continue;
            }
        }
        } catch (IOException e) {
            System.err.println("Unable to listen on port " + port);
            System.exit(1);
        } catch(IllegalArgumentException e){
            System.err.println("Unable to listen on port " + port);
            System.exit(1);
        }finally{
            if (srvSock != null) {
                try {
                    srvSock.close();
                } catch (IOException e) {
                    System.err.println("Server socket close exception!");
                    System.exit(1);
                }
            }
        }
    }
}
