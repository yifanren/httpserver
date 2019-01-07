#include "HTTPServer.h"
#include <stdlib.h>

#define PORT "1234"

static HTTPServer* svr;
int main(void)
{
    // Instance and start the server
    svr = new HTTPServer(atoi(PORT));
    if (!svr->start()) {
        svr->stop();
        return -1;
    }

    // Run main event loop
    svr->process();

    // Stop the server
    svr->stop();

    // Delete server

    return 0;
}
