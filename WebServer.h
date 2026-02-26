#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

class WebServer {
private:
    Request currentRequest;
    bool busy;
    int remainingTime;
    int serverId;

public:
    WebServer(int id) : busy(false), remainingTime(0), serverId(id) {}
    void startRequest(Request& request);
    bool isFinished();
    void iterate();
};

#endif // WEBSERVER_H