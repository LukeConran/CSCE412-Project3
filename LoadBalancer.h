#ifndef LOADBALANCER_H
#define LOADBALANCER_H

/**
 * @file LoadBalancer.h
 * @brief Declares the load balancer responsible for request routing and scaling.
 */

#include <string>
#include "WebServer.h"
#include "Request.h"
#include <vector>
#include <queue>

/**
 * @brief Manages server pool, request queue, and request filtering.
 */
class LoadBalancer {
private:
    /** @brief Pool of active web servers. */
    std::vector<WebServer> servers;
    /** @brief Simulation time tracker (reserved for cycle-based state). */
    int currentTime;
    /** @brief FIFO queue of pending requests awaiting service. */
    std::queue<Request> requestQueue;
    /** @brief List of blocked source IP prefixes. */
    std::vector<std::string> blockedPrefixes;
public:

    /** @brief Constructs an empty load balancer. */
    LoadBalancer() : currentTime(0) {}

    /** @brief Advances all scheduling and server execution by one cycle. */
    void step();

    /**
     * @brief Attempts to enqueue a request if its source is not blocked.
     * @param request Request to enqueue.
     * @return true when accepted; false when rejected by IP filter.
     */
    bool addRequest(const Request& request);

    /**
     * @brief Creates a random request using configured timing bounds.
     * @return Randomly generated request object.
     */
    Request generateRandomRequest();

    /** @brief Adds or removes servers based on queue thresholds. */
    void adjustServerCount();

    /** @brief Adds one server to the active pool. */
    void addServer();

    /** @brief Removes one server from the active pool when possible. */
    void removeServer();

    /**
     * @brief Checks whether an IP is blocked by configured prefixes.
     * @param ip Source IP string to validate.
     * @return true if blocked; false otherwise.
     */
    bool isBlocked(const std::string& ip);

    /**
     * @brief Seeds the queue with randomly generated requests.
     * @param count Number of requests to generate.
     */
    void initializeQueue(int count);

    /**
     * @brief Gets the current number of active servers.
     * @return Number of servers in the pool.
     */
    int getServerCount() const { return (int)servers.size(); }

    /**
     * @brief Gets the current request queue length.
     * @return Number of pending requests.
     */
    int getQueueSize() const { return (int)requestQueue.size(); }

    /**
     * @brief Adds a blocked IP prefix to the deny list.
     * @param prefix Prefix used for source IP filtering.
     */
    void addBlockedPrefix(const std::string& prefix) { blockedPrefixes.push_back(prefix); }
};




#endif // LOADBALANCER_H