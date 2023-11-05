#ifndef CSERVER_TANAYALPKONUR_SERVER_H
#define CSERVER_TANAYALPKONUR_SERVER_H

#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

int createServerSocket();

int bindServerSocket(int serverSocket, struct sockaddr_in *serverAddress);

void serveFile(int clientSocket, const std::string& filename);

#endif //CSERVER_TANAYALPKONUR_SERVER_H
