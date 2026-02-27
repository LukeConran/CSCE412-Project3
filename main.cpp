#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "LoadBalancer.h"
#include "Request.h"
#include "Config.h"

/**
 * @file main.cpp
 * @brief Runs the load balancer simulation and writes event logs and summary output.
 */

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_RED     "\033[31m"
#define COLOR_MAGENTA "\033[35m"

/**
 * @brief Program entry point for the load balancer simulation.
 *
 * Initializes configuration, creates the server pool, runs simulation cycles,
 * applies request generation and dynamic scaling, and writes results to the log.
 *
 * @return 0 on success, non-zero when log initialization fails.
 */
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::ofstream log(LOG_FILE);
    if (!log.is_open()) {
        std::cerr << COLOR_RED << "[ERROR]" << COLOR_RESET
                  << " Could not open log file: " << LOG_FILE << "\n";
        return 1;
    }

    std::ostringstream cfgSummary;
    cfgSummary << "\n===== Load Balancer Configuration =====\n"
               << "  Servers:              " << NUM_SERVERS                                     << "\n"
               << "  Simulation Time:      " << SIMULATION_TIME                                 << " cycles\n"
               << "  Initial Queue Size:   " << NUM_SERVERS * INITIAL_QUEUE_MULTIPLIER          << " requests\n"
               << "  Queue Min Threshold:  " << MIN_QUEUE_PER_SERVER                            << " * servers\n"
               << "  Queue Max Threshold:  " << MAX_QUEUE_PER_SERVER                            << " * servers\n"
               << "  Adjust Cooldown:      " << SERVER_ADJUST_COOLDOWN                          << " cycles\n"
               << "  New Request Interval: " << NEW_REQUEST_INTERVAL                            << " cycles\n"
               << "  Range For Task Times: " << REQUEST_TIME_MIN << " - " << REQUEST_TIME_MAX   << " cycles\n"
               << "  Log File:             " << LOG_FILE                                        << "\n"
               << "  Blocked Prefixes:     ";
    for (size_t i = 0; i < BLOCKED_PREFIXES.size(); i++) {
        cfgSummary << BLOCKED_PREFIXES[i];
        if (i + 1 < BLOCKED_PREFIXES.size()) cfgSummary << ", ";
    }
    cfgSummary << "\n========================================\n\n";

    std::cout << cfgSummary.str();
    log       << cfgSummary.str();

    /////////////////////////////////////////////////////////////////////////////////////////////////

    LoadBalancer lb;

    for (const auto& prefix : BLOCKED_PREFIXES) {
        lb.addBlockedPrefix(prefix);
    }

    for (int i = 0; i < NUM_SERVERS; i++) {
        lb.addServer();
    }

    int initialQueueSize = NUM_SERVERS * INITIAL_QUEUE_MULTIPLIER;
    lb.initializeQueue(initialQueueSize);

    int totalRequestsGenerated = initialQueueSize;
    int totalRequestsBlocked   = 0;
    int scaleUpEvents          = 0;
    int scaleDownEvents        = 0;
    int cooldownTimer          = 0;

    std::cout << COLOR_GREEN << "[START]" << COLOR_RESET
              << " Simulation starting for " << SIMULATION_TIME << " clock cycles...\n\n";
    log << "[START] Simulation running for " << SIMULATION_TIME << " clock cycles.\n\n";

    //////////////////////////////////////////////////////////////////////////////////////////////////

    for (int cycle = 1; cycle <= SIMULATION_TIME; cycle++) {

        if (cycle % NEW_REQUEST_INTERVAL == 0) {
            Request r = lb.generateRandomRequest();
            totalRequestsGenerated++;
            if(!lb.addRequest(r)) {
                totalRequestsBlocked++;
                std::string msg = "[CYCLE " + std::to_string(cycle) + "] BLOCKED → "
                                + r.ipIn + " | Queue: "
                                + std::to_string(lb.getQueueSize()) + "\n";
                log << msg;
            }
        }

        lb.step();

        if (cooldownTimer > 0) {
            cooldownTimer--;
        } else {
            int prevCount = lb.getServerCount();
            lb.adjustServerCount();
            int newCount = lb.getServerCount();

            if (newCount > prevCount) {
                scaleUpEvents++;
                cooldownTimer = SERVER_ADJUST_COOLDOWN;
                std::string msg = "[CYCLE " + std::to_string(cycle) + "] SCALE UP → "
                                + std::to_string(newCount) + " servers | Queue: "
                                + std::to_string(lb.getQueueSize()) + "\n";
                log << msg;

            } else if (newCount < prevCount) {
                scaleDownEvents++;
                cooldownTimer = SERVER_ADJUST_COOLDOWN;
                std::string msg = "[CYCLE " + std::to_string(cycle) + "] SCALE DOWN → "
                                + std::to_string(newCount) + " servers | Queue: "
                                + std::to_string(lb.getQueueSize()) + "\n";
                log << msg;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////

    std::ostringstream summary;
    summary << "\n"
            << "============= SIMULATION SUMMARY =============\n"
            << "  Total Clock Cycles Run:     " << SIMULATION_TIME                                  << "\n"
            << "  Total Requests Generated:   " << totalRequestsGenerated                           << "\n"
            << "  Total Requests Blocked:     " << totalRequestsBlocked                             << "\n"
            << "  Requests Processed:         " << totalRequestsGenerated - totalRequestsBlocked    << "\n"
            << "  Scale-Up Events:            " << scaleUpEvents                                    << "\n"
            << "  Scale-Down Events:          " << scaleDownEvents                                  << "\n"
            << "  Final Server Count:         " << lb.getServerCount()                              << "\n"
            << "  Remaining Queue Size:       " << lb.getQueueSize()                                << "\n"
            << "==============================================\n";

    std::cout << summary.str();
    log       << summary.str();

    log.close();
    std::cout << COLOR_GREEN << "\n[DONE]" << COLOR_RESET
              << " Log written to " << LOG_FILE << "\n";

    return 0;
}