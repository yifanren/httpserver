#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <netinet/in.h>
#include <arpa/inet.h>
class Client{
    int socketDesc;
    sockaddr_in clientAddr;

    public:
    Client(int fd, sockaddr_in addr);
    ~Client();

    sockaddr_in getClientAddr() {
        return clientAddr;
    }

    int getSocket() {
        return socketDesc;
    }

    char* getClientIP() {
        return inet_ntoa(clientAddr.sin_addr);
    }
};

#endif
