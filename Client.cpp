#include "Client.h"

Client::Client(int fd, sockaddr_in addr) {
    socketDesc = fd;
    clientAddr = addr;
}

Client::~Client() {

}


