#ifndef WEBSERVER_H
#define WEBSERVER_H

/**
 * @file WebServer.h
 * @brief Declares the simulated web server that processes queued requests.
 */

#include "Request.h"

/**
 * @brief Simulates a single server that can process one request at a time.
 */
class WebServer {
private:
    /** @brief Request currently assigned to this server. */
    Request currentRequest;
    /** @brief Indicates whether the server is currently processing a request. */
    bool busy;
    /** @brief Remaining cycles until the current request is complete. */
    int remainingTime;
    /** @brief Stable identifier for this server instance. */
    int serverId;

public:
    /**
     * @brief Constructs a server with a specific identifier.
     * @param id Unique identifier for the server.
     */
    WebServer(int id) : busy(false), remainingTime(0), serverId(id) {}

    /**
     * @brief Assigns a request to the server and begins processing.
     * @param request Request to start processing.
     */
    void startRequest(Request& request);

    /**
     * @brief Reports whether the server is idle or done with its current request.
     * @return true if no active work remains; false otherwise.
     */
    bool isFinished();

    /**
     * @brief Advances processing by one simulation cycle.
     */
    void iterate();
};

#endif // WEBSERVER_H