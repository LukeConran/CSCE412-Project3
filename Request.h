#ifndef REQUEST_H
#define REQUEST_H

/**
 * @file Request.h
 * @brief Defines the request model used by servers and the load balancer.
 */

#include <string>

/**
 * @brief Represents a single workload request in the simulation.
 */
struct Request {
    /** @brief Source IP address of the incoming request. */
    std::string ipIn;
    /** @brief Destination IP address targeted by the request. */
    std::string ipOut;
    /** @brief Number of cycles required to complete the request. */
    int time;
    /** @brief Request category marker (for example 'P' or 'S'). */
    char jobType;
};

#endif // REQUEST_H