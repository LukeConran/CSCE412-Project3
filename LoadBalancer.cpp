#include "LoadBalancer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Config.h"

/**
 * @file LoadBalancer.cpp
 * @brief Implements load balancer behavior for request routing, scaling, and filtering.
 */

/**
 * @brief Advances the simulation by one tick.
 *
 * Assigns queued requests to any idle server, then iterates each server once to
 * progress active work.
 */
void LoadBalancer::step() {
    for (auto& server : servers) {
        if (server.isFinished() && !requestQueue.empty()) {
            Request nextRequest = requestQueue.front();
            requestQueue.pop();
            server.startRequest(nextRequest);
        }
    }

    for(auto& server : servers) {
        server.iterate();
    }
}

/**
 * @brief Adds a request to the queue when the source IP is allowed.
 *
 * @param request Request to enqueue.
 * @return true if the request is accepted and queued; false if blocked.
 */
bool LoadBalancer::addRequest(const Request& request) {
    if (!isBlocked(request.ipIn)) {
        requestQueue.push(request);
        return true;
    } else {
        std::cout << "\033[31m[BLOCKED]\033[0m Request from " << request.ipIn << " was denied.\n";
        return false;
    }
}

/**
 * @brief Generates a random request for simulation traffic.
 *
 * Produces random source/destination IP addresses, a processing time within
 * configured bounds, and a random job type.
 *
 * @return Randomly generated Request instance.
 */
Request LoadBalancer::generateRandomRequest() {
    auto randomIp = []() {
        return std::to_string(rand() % 256) + "." +
               std::to_string(rand() % 256) + "." +
               std::to_string(rand() % 256) + "." +
               std::to_string(rand() % 256);
    };
    Request req;
    req.ipIn = randomIp();
    req.ipOut = randomIp();
    req.time = rand() % (REQUEST_TIME_MAX - REQUEST_TIME_MIN + 1) + REQUEST_TIME_MIN;
    req.jobType = (rand() % 2 == 0) ? 'P' : 'S';
    return req;
}

/**
 * @brief Dynamically scales the number of servers based on queue pressure.
 *
 * Adds a server if the queue exceeds the upper threshold per server, or removes
 * one (while keeping at least one server) when load falls below the lower threshold.
 */
void LoadBalancer::adjustServerCount() {
    int totalQueueSize = (int)requestQueue.size();
    int serverCount = (int)servers.size();

    if (totalQueueSize > MAX_QUEUE_PER_SERVER * serverCount) {
        addServer();
    } else if (totalQueueSize < MIN_QUEUE_PER_SERVER * serverCount && serverCount > 1) {
        removeServer();
    }
}

/**
 * @brief Adds one server instance to the load balancer.
 */
void LoadBalancer::addServer() {
    servers.emplace_back((int)servers.size() + 1);
    std::cout << "\033[33m[SCALE UP]\033[0m Server added. Total servers: " << servers.size() << "\n";
}

/**
 * @brief Removes one server instance when more than one server exists.
 */
void LoadBalancer::removeServer() {
    if ((int)servers.size() > 1) {
        servers.pop_back();
        std::cout << "\033[36m[SCALE DOWN]\033[0m Server removed. Total servers: " << servers.size() << "\n";
    }
}

/**
 * @brief Checks whether an IP address matches any blocked prefix.
 *
 * @param ip Source IP address to evaluate.
 * @return true if blocked; false otherwise.
 */
bool LoadBalancer::isBlocked(const std::string& ip) {
    for (const auto& prefix : blockedPrefixes) {
        if (ip.substr(0, prefix.size()) == prefix) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Pre-populates the request queue with random traffic.
 *
 * @param count Number of random requests to generate and attempt to enqueue.
 */
void LoadBalancer::initializeQueue(int count) {
    for (int i = 0; i < count; ++i) {
        Request r = generateRandomRequest();
        addRequest(r);
    }
    std::cout << "\033[32m[INIT]\033[0m Queue initialized with " << count << " requests.\n";
}
