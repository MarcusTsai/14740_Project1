import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.URLDecoder;

/**
 * This function is designed to handle
 * the request from the client.
 * Basically, the design only support GET and HEAD command
 * in the request header. If there are other commands within
 * request, the response header will reply with status code
 * 501, "Not Implemented!". In addition, the design also
 * will instruct 404 since the file is not found or 400 if there
 * is no command or incorrect request header.
 */
public class ClientHandler implements Runnable {
    private int statusCode;
    private Socket clientSock;
    private BufferedReader inStream = null;
    private DataOutputStream outStream = null;
    private StringBuilder headers;

    public ClientHandler(Socket aClientSock) {
        this.clientSock = aClientSock;
        headers = new StringBuilder();
        statusCode = 501;
    }

    private void response(File file, boolean isFile) throws IOException {

        StringBuilder response = new StringBuilder();
        String statusLine = null;
        String serverInfo = new String("Server: Simple/1.0\r\n");
        String contentType = null;
        String content = null;

        /*
         * Generate header & content based on status code
         */
        switch (statusCode) {
            case 200:
                statusLine = new String("HTTP/1.0 200 OK\r\n");
                try {
                    contentType = "Content-Type: " + GetMime.getMimeType(file.getName()) + "\r\n";
                } catch (IOException e) {
                    contentType = "Content-Type: \r\n";
                }
                if (!isFile) {
                    content = new String("");
                }
                break;

            case 400:
                statusLine = new String("HTTP/1.0 400 Bad Request\r\n");
                contentType = "Content-Type: text/html\r\n";
                content = "<html>" + "<title>400</title>" + "<body>" + "<h1>400 Bad Request</h1><BR>" + "</body>"
                        + "</html>";
                break;

            case 404:
                statusLine = new String("HTTP/1.0 404 Not Found\r\n");
                contentType = "Content-Type: text/html\r\n";
                content = "<html>" + "<title>404</title>" + "<body>" + "<h1>404 Not Found</h1><BR>" + "</body>" + "</html>";
                break;

            default:
                statusLine = new String("HTTP/1.0 501 Not Implemented\r\n");
                contentType = "Content-Type: text/html\r\n";
                content = "<html>" + "<title>501</title>" + "<body>" + "<h1>501 Not Implemented</h1><BR>" + "</body>"
                        + "</html>";
                break;
        }
        /*
         * Send back the header
         */
        response.append(statusLine);
        response.append(serverInfo);
        response.append(contentType);
        response.append("Connection: close\r\n");
        response.append("\r\n");

        /*
         * Check if the content should be sent back
         */
        if (isFile) {
            BufferedReader reader = null;
            try {
                reader = new BufferedReader(new FileReader(file));
                String currentLine = null;
                while ((currentLine = reader.readLine()) != null) {
                    response.append(currentLine);
                }
            } catch (FileNotFoundException e) {
                System.err.println("File not found exception!");
            } catch (IOException e) {
                System.err.println("IO exception!");
            } finally {
                try {
                    reader.close();
                } catch (IOException e) {
                    System.err.println("Reader close exception!");
                    e.printStackTrace();
                }
            }
        } else {
            response.append(content);
        }
        /*
         * Send response back and flush the stream to make sure that the
         * data is sent immediately
         */
        outStream.writeBytes(response.toString());
        outStream.flush();
    }

    @Override
    public void run() {
        try {
            inStream = new BufferedReader(new InputStreamReader(clientSock.getInputStream()));
            outStream = new DataOutputStream(clientSock.getOutputStream());
            /*
             * Read the first line of the data send by the client
             */
            String request = inStream.readLine();
            if (request == null) {
                statusCode = 400;
                response(null, false);
                return;
            }
            /*
             * Parse header
             */
            String[] requestItems = request.split(" ");

            /* Read the data send by the client */
            String buffer = null;
            while (inStream.ready() && ((buffer = inStream.readLine()) != null)) {
                headers.append(buffer);
            }
            System.out.println(
                    "Read from client " + clientSock.getInetAddress() + ":" + clientSock.getPort() + " " + request);


            if (requestItems.length != 3 || (requestItems[1].charAt(0) != '/')) {
                /*
                 * Bad request
                 */
                statusCode = 400;
                response(null, false);
            } else {
                String fileName = null;
                File file = null;
                switch (requestItems[0]) {

                    /*
                     * Get request
                     */
                    case "GET":
                        if (requestItems[1].equals("/")) {
                        /*
                         * if uri is "/", then response the default home page
                         * index.html
                         */
                            fileName = new String("index.html");
                        } else {
                            fileName = requestItems[1].replaceFirst("/", "");

                            /*
                             * URL decode
                             */
                            fileName = URLDecoder.decode(fileName, "UTF-8");
                        }

                        file = new File(fileName);
                        if (file.isFile()) {
                            statusCode = 200;
                            response(file, true);
                        } else {
                            statusCode = 404;
                            response(null, false);
                        }
                        break;
                    /*
                     * HEAD request, only need to send header back
                     */
                    case "HEAD":
                        if (requestItems[1].equals("/")) {
                            statusCode = 200;
                            response(null, false);
                        } else {
                            fileName = requestItems[1].replaceFirst("/", "");
                            fileName = URLDecoder.decode(fileName, "UTF-8");
                            file = new File(fileName);
                            if (file.isFile()) {
                                statusCode = 200;
                                response(file, false);
                            } else {
                                statusCode = 404;
                                response(null, false);
                            }
                        }
                        break;
                    default:
                        statusCode = 501;
                        response(null, false);
                        break;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                outStream.close();
            } catch (IOException e1) {
                System.err.println("OutputStream close error!");
                e1.printStackTrace();
            }
            try {
                inStream.close();
            } catch (IOException e1) {
                System.err.println("InputStream close error!");
                e1.printStackTrace();
            }
            try {
                /*
                 * Interaction with this client complete, close() the socket
                 */
                clientSock.close();
            } catch (IOException e) {
                System.err.println("Socket close error!");
                e.printStackTrace();
            }

        }
    }
}
