#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

const int NUM_SERVERS               = 10;
const int SIMULATION_TIME           = 10000;
const int INITIAL_QUEUE_MULTIPLIER  = 100;

const int MIN_QUEUE_PER_SERVER      = 50;
const int MAX_QUEUE_PER_SERVER      = 80;
const int SERVER_ADJUST_COOLDOWN    = 200;

const int NEW_REQUEST_INTERVAL      = 3;
const int REQUEST_TIME_MIN          = 5;
const int REQUEST_TIME_MAX          = 20;

const std::string LOG_FILE  = "loadbalancer_log.txt";

const std::vector<std::string> BLOCKED_PREFIXES = {
    "192.16",
    "10.0.",
    "187.15"
};

#endif // CONFIG_H