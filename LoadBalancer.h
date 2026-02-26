#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <string>
#include "WebServer.h"
#include "Request.h"
#include <vector>
#include <queue>

class LoadBalancer {
private:
    std::vector<WebServer> servers;
    int currentTime;
    std::queue<Request> requestQueue;
    std::vector<std::string> blockedPrefixes;
public:
    LoadBalancer() : currentTime(0) {}
    void step();
    bool addRequest(const Request& request);
    Request generateRandomRequest();
    void adjustServerCount();
    void addServer();
    void removeServer();
    bool isBlocked(const std::string& ip);
    void initializeQueue(int count);
    int getServerCount() const { return (int)servers.size(); }
    int getQueueSize() const { return (int)requestQueue.size(); }
    void addBlockedPrefix(const std::string& prefix) { blockedPrefixes.push_back(prefix); }
};




#endif // LOADBALANCER_H