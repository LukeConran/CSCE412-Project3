#include "WebServer.h"
#include <iostream>

/**
 * @file WebServer.cpp
 * @brief Implements request lifecycle behavior for a simulated web server.
 */

/**
 * @brief Starts processing a request on this server.
 * @param request Request assigned to this server.
 */
void WebServer::startRequest(Request& request) {
    currentRequest = request;
    busy = true;
    remainingTime = request.time;
}

/**
 * @brief Indicates whether this server has completed its assigned work.
 * @return true if the server is idle or its request has finished; false otherwise.
 */
bool WebServer::isFinished() {
    return !busy || remainingTime <= 0;
}

/**
 * @brief Advances the active request by one cycle and releases the server when done.
 */
void WebServer::iterate() {
    if (busy) {
        remainingTime--;
        if (remainingTime <= 0) {
            busy = false;
        }
    }
}