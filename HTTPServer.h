#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#include <unordered_map>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include "Client.h"

#define INVALID_SOCKET -1

class HTTPServer{
    // Server Socket
    int listenPort;
    int listenSocket;
    struct sockaddr_in serverAddr;

    // Select
    struct timeval timeout;
    int maxfd;
    fd_set read_fd;

    // Client Map <fd, client>
    std::unordered_map<int, Client*> clientMap;

    // Connection processing
    void acceptConnection();
    void disconnectClient(Client* cl, bool mapErase = true);

    public:
    HTTPServer(int port);
    ~HTTPServer();

    bool start();
    void stop();
    void process();
};

#endif
