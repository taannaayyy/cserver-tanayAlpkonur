#include "server.h"

// creates a socket for the server.
// uses the socket() system call to create a socket with the specified address family (AF_INET),
// socket type (SOCK_STREAM for TCP)
// and protocol (0 for default protocol).
int createServerSocket() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        exit(1);
    }
    return serverSocket;
}
// function to bind the server socket to a specific address and port
int bindServerSocket(int serverSocket, struct sockaddr_in *serverAddress) {
    // set up the server address information
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_port = htons(8080);
    serverAddress->sin_addr.s_addr = INADDR_ANY;

    // attempt to bind the server socket
    if (bind(serverSocket, (struct sockaddr *)serverAddress, sizeof(*serverAddress)) < 0) {
        return -1;
    }
    // return 0 if binding is successful
    return 0;
}

// function to serve a file to a client
void serveFile(int clientSocket, const std::string& filename) {
    // open the file in binary mode
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    // check if the file was successfully opened
    if (file.is_open()) {
        // get the file size
        std::streampos fileSize = file.tellg();
        // set the file position indicator to the beginning
        file.seekg(0, std::ios::beg);

        std::string contentType;

        // determine the content type based on the file extension
        if (filename.find(".html") != std::string::npos) {
            contentType = "text/html";
        } else if (filename.find(".css") != std::string::npos) {
            contentType = "text/css";
        } else if (filename.find(".gif") != std::string::npos) {
            contentType = "image/gif";
        } else {
            contentType = "text/plain";
        }
        // construct the HTTP response header
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(fileSize) + "\r\nContent-Type: " + contentType + "; charset=utf-8\r\n\r\n";

        // send the response header to the client
        send(clientSocket, response.c_str(), response.size(), 0);

        char buffer[1024];
        while (!file.eof()) {
            // read a chunk of the file
            file.read(buffer, sizeof(buffer));
            // send the chunk to the client
            send(clientSocket, buffer, file.gcount(), 0);
        }
        // if the file cannot be opened, it responds with an HTTP 404 Not Found error.
    } else {
        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(clientSocket, response.c_str(), response.size(), 0);
    }
}


int main() {

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t addrLen = sizeof(clientAddress);

    // function to create server socket
    serverSocket = createServerSocket();

    // bind the server socket to an address and port
    if (bindServerSocket(serverSocket, &serverAddress) < 0) {
        perror("Error binding socket");
        return 1;
    }

    // start listening for incoming connections
    if (listen(serverSocket, 5) < 0) {
        perror("Error listening");
        return 1;
    }

    while (1) {
        // Accept incoming connection
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &addrLen);
        if (clientSocket < 0) {
            perror("Error accepting connection");
            return 1;
        }

        char request[1024];
        // receive the HTTP request from the client
        recv(clientSocket, request, sizeof(request), 0);

        std::istringstream requestStream(request);
        std::string requestLine;
        // get the first line of the request
        getline(requestStream, requestLine);

        // parse request to determine which file to serve
        if (requestLine.find("GET") != std::string::npos) {
            size_t start = requestLine.find_first_of(" ");
            size_t end = requestLine.find_last_of(" ");
            std::string requestedFile = requestLine.substr(start+1, end-start-1);

            if (requestedFile == "/") {
                serveFile(clientSocket, "site/index.html"); // serve the index.html file
            } else if (requestedFile == "/style.css") {
                serveFile(clientSocket, "site/style.css"); // serve the style.css file
            } else if (requestedFile == "/n46h9ay1rd291.gif") {
                serveFile(clientSocket, "site/n46h9ay1rd291.gif"); // serve .gif file
            } else {
                std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                send(clientSocket, response.c_str(), response.size(), 0); // send a 404 Not Found response
            }
        }
        // close the client socket
        close(clientSocket);
    }
    // close the server socket
    close(serverSocket);
    // exit the program with a success code
    return 0;
}