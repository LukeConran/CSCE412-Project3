#include "LoadBalancer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Config.h"

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

void LoadBalancer::addRequest(const Request& request) {
    if (!isBlocked(request.ipIn)) {
        requestQueue.push(request);
    } else {
        std::cout << "\033[31m[BLOCKED]\033[0m Request from " << request.ipIn << " was denied.\n";
    }
}

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
    req.time = rand() % 20 + 1;
    req.jobType = (rand() % 2 == 0) ? 'P' : 'S';
    return req;
}

void LoadBalancer::adjustServerCount() {
    int totalQueueSize = (int)requestQueue.size();
    int serverCount = (int)servers.size();

    if (totalQueueSize > MAX_QUEUE_PER_SERVER * serverCount) {
        addServer();
    } else if (totalQueueSize < MIN_QUEUE_PER_SERVER * serverCount && serverCount > 1) {
        removeServer();
    }
}

void LoadBalancer::addServer() {
    WebServer newServer;
    servers.push_back(newServer);
    std::cout << "\033[33m[SCALE UP]\033[0m Server added. Total servers: " << servers.size() << "\n";
}

void LoadBalancer::removeServer() {
    if ((int)servers.size() > 1) {
        servers.pop_back();
        std::cout << "\033[36m[SCALE DOWN]\033[0m Server removed. Total servers: " << servers.size() << "\n";
    }
}

bool LoadBalancer::isBlocked(const std::string& ip) {
    for (const auto& prefix : blockedPrefixes) {
        if (ip.substr(0, prefix.size()) == prefix) {
            return true;
        }
    }
    return false;
}

void LoadBalancer::initializeQueue(int count) {
    for (int i = 0; i < count; ++i) {
        Request r = generateRandomRequest();
        addRequest(r);
    }
    std::cout << "\033[32m[INIT]\033[0m Queue initialized with " << count << " requests.\n";
}
