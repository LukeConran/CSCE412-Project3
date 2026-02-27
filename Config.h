#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file Config.h
 * @brief Centralized simulation constants and blocked IP prefix configuration.
 */

#include <string>
#include <vector>

/** @brief Initial number of web servers to create before simulation starts. */
const int NUM_SERVERS               = 10;
/** @brief Total number of simulation cycles to run. */
const int SIMULATION_TIME           = 10000;
/** @brief Initial queue size multiplier per server. */
const int INITIAL_QUEUE_MULTIPLIER  = 100;

/** @brief Lower queue threshold per server used to trigger scale-down checks. */
const int MIN_QUEUE_PER_SERVER      = 50;
/** @brief Upper queue threshold per server used to trigger scale-up checks. */
const int MAX_QUEUE_PER_SERVER      = 80;
/** @brief Cooldown cycles between successive scaling actions. */
const int SERVER_ADJUST_COOLDOWN    = 200;

/** @brief Number of cycles between random request generation events. */
const int NEW_REQUEST_INTERVAL      = 3;
/** @brief Minimum processing time assigned to generated requests. */
const int REQUEST_TIME_MIN          = 5;
/** @brief Maximum processing time assigned to generated requests. */
const int REQUEST_TIME_MAX          = 20;

/** @brief Output log file path for simulation events and summary data. */
const std::string LOG_FILE  = "loadbalancer_log.txt";

/** @brief Source IP prefixes that should be denied by the load balancer. */
const std::vector<std::string> BLOCKED_PREFIXES = {
    "192.16",
    "10.0.",
    "187.15"
};

#endif // CONFIG_H