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
    FD_CLR(listenSocket, &read_fd);
    close(listenSocket);
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
                //printf("have info...\n");
                break;
            }
        }
        if (FD_ISSET(listenSocket, &tempSet)) {
            acceptConnection();
            printf("have client connect...\n");
        }
    }
}

void HTTPServer::acceptConnection() {
    // Setup new client with prelim address info
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int clfd = INVALID_SOCKET;

    // Accept the pending connection and retrive the client descriptor
    clfd = accept(listenSocket, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
    if(clfd == INVALID_SOCKET)
        return;

    // Instance Client object
    Client *cl = new Client(clfd, clientAddr);

    // Add the client object to the client map
    clientMap.insert(std::pair<int, Client*>(clfd, cl));

    // Select maxfd
    FD_SET(clfd, &read_fd);
    if (maxfd < clfd)
        maxfd = clfd;

    // Print the client's IP on connect
    std::cout << "[" << cl->getClientIP() << "] connected" << std::endl;
}

void HTTPServer::disconnectClient(Client *cl, bool mapErase) {
    if(cl == NULL)
        return;

    std::cout << "[" << cl->getClientIP() << "] disconnected" << std::endl;

    // Remove socket events from select
    FD_CLR(cl->getSocket(), &read_fd);

    // Close the socket descriptor
    close(cl->getSocket());

    // Remove the client from the clientMap
    if(mapErase)
        clientMap.erase(cl->getSocket());

    // Delete the client object from memory
    delete cl;
}

