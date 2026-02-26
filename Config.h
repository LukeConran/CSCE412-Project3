#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

const int NUM_SERVERS               = 10;
const int SIMULATION_TIME           = 10000;
const int INITIAL_QUEUE_MULTIPLIER  = 100;

const int MIN_QUEUE_PER_SERVER      = 50;
const int MAX_QUEUE_PER_SERVER      = 80;
const int SERVER_ADJUST_COOLDOWN    = 10;

const int NEW_REQUEST_INTERVAL      = 1;
const int REQUEST_TIME_MIN          = 5;
const int REQUEST_TIME_MAX          = 20;

const std::string LOG_FILE  = "loadbalancer_log.txt";

const std::vector<std::string> BLOCKED_PREFIXES = {
    "10.0.0",
    "123.43.21",
    "123.45.67"
};

#endif // CONFIG_H