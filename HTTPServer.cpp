#include "HTTPServer.h"

HTTPServer::HTTPServer(int port) {
    listenSocket = INVALID_SOCKET;
    listenPort = port;
    maxfd = INVALID_SOCKET;
}

HTTPServer::~HTTPServer() {

}

bool HTTPServer::start() {
    // Create a handle for the listening socket, TCP
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenSocket == INVALID_SOCKET) {
        std::cout << "Could not create socket!" << std::endl;
        return false;
    }

    // Populate the server address structure
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(listenPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind: Assign the address to the socket
    if(bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        std::cout << "Failed to bind to the address!" << std::endl;
        return false;
    }

    // Listen: Put the socket in a listening state, ready to accept connections
    if(listen(listenSocket, SOMAXCONN) != 0) {
        std::cout << "Failed to put the socket in a listening state" << std::endl;
        return false;
    }

    //set Socket to fd_set
    FD_ZERO(&read_fd);
    FD_SET(listenSocket, &read_fd);
    maxfd = listenSocket;

    std::cout << "Server ready. Listening on port " << listenPort << "..." << std::endl;
    return true;
}

void HTTPServer::stop() {
    std::cout << "Server shutdown!" << std::endl;
}

void HTTPServer::process() {
    int nev = 0;
    Client* cl = NULL;
    fd_set tempSet;
    std::unordered_map<int, Client*>::iterator it;

    while(1) {
        FD_ZERO(&tempSet);
        tempSet = read_fd;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // Get a list of changed socket descriptors with a read event triggered in evList
        nev = select(maxfd + 1, &tempSet, NULL, NULL, &timeout);
        int curFd = 0;
        if (nev < 0) {
            printf("select failed\n");
            break;
        }

        if(nev == 0) {
            printf("timeout\n");
            continue;
        }

        for ( it = clientMap.begin(); it != clientMap.end(); it++) {
            if (FD_ISSET(it->first, &tempSet)) {
                cl = it->second;
                //readClient(cl);
                printf("have info...\n");
            }
        }
        if (FD_ISSET(listenSocket, &tempSet)) {
            //acceptConnection();
            printf("have client connect...\n");
        }
    }
}
