#include "WebServer.h"
#include <iostream>

void WebServer::startRequest(Request& request) {
    currentRequest = request;
    busy = true;
    remainingTime = request.time;
}

bool WebServer::isFinished() {
    return !busy || remainingTime <= 0;
}

void WebServer::iterate() {
    if (busy) {
        remainingTime--;
        if (remainingTime <= 0) {
            busy = false;
        }
    }
}