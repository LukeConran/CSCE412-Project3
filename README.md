# CSCE412 Project 3 — Load Balancer Simulation (C++)

This repository contains a simple **load balancer simulation** written in **C++17**. The program simulates a pool of `WebServer` instances processing a stream of randomly-generated requests. Requests are queued, dispatched to idle servers, and optionally **blocked** based on IP prefix rules.

## What it builds

The included `Makefile` compiles three implementation files into a single executable:

- `main.cpp`
- `LoadBalancer.cpp`
- `WebServer.cpp`

Output binary:
- `loadbalancer`

## How it works (high level)

### Requests (`Request.h`)
A request contains:

- `ipIn`: source IP address (randomly generated)
- `ipOut`: destination IP address (randomly generated)
- `time`: how many simulation cycles it takes to complete (random duration)
- `jobType`: either `'P'` or `'S'` (randomly assigned)

### Web servers (`WebServer.h/.cpp`)
A `WebServer` can run one request at a time:

- `startRequest(...)` assigns the server a request and marks it busy
- `iterate()` decrements the remaining time each cycle
- `isFinished()` reports whether the server is idle / done

### Load balancer (`LoadBalancer.h/.cpp`)
The `LoadBalancer` owns:

- a vector of `WebServer` objects
- a FIFO queue of pending `Request`s
- a list of blocked IP prefixes

Key behaviors:

- **Dispatching:** each cycle, any server that has finished will pull the next request from the queue (if available).
- **Processing:** each cycle, all servers call `iterate()` to advance work.
- **Blocking:** when a request is added, its `ipIn` is checked against the blocked prefix list; blocked requests are denied and not enqueued.
- **Initialization:** the queue can be pre-filled with an initial batch of random requests.

## Configuration (`Config.h`)

Simulation parameters are compile-time constants in `Config.h`, including:

- `NUM_SERVERS`: starting server count
- `SIMULATION_TIME`: number of cycles to simulate
- `INITIAL_QUEUE_MULTIPLIER`: initial queue size = `NUM_SERVERS * INITIAL_QUEUE_MULTIPLIER`
- Queue thresholds:
  - `MIN_QUEUE_PER_SERVER`
  - `MAX_QUEUE_PER_SERVER`
  - `SERVER_ADJUST_COOLDOWN` (cooldown concept for scaling decisions)
- Request generation:
  - `NEW_REQUEST_INTERVAL`
  - `REQUEST_TIME_MIN`, `REQUEST_TIME_MAX`
- Logging:
  - `LOG_FILE` (defaults to `loadbalancer_log.txt`)
- Security / filtering:
  - `BLOCKED_PREFIXES` (list of IP prefixes to deny)

At startup, the program prints a configuration summary to both stdout and the log file.

## Build and run

### Build
```bash
make
```

### Run
```bash
./loadbalancer
```

### Clean
```bash
make clean
```

## Output

During the simulation you’ll see status messages such as:

- simulation start information
- blocked request notices (`[BLOCKED] ...`)
- server scaling messages (`[SCALE UP]`, `[SCALE DOWN]`) when scaling functions are triggered

A copy of the configuration summary (and other log output written by `main.cpp`) is written to `loadbalancer_log.txt`.

## File overview

- `main.cpp`: program entry point; prints configuration; sets up load balancer, blocked prefixes, servers, and initial queue; runs the simulation loop.
- `LoadBalancer.h/.cpp`: core queueing, dispatching, request generation, blocking, and (optional) scaling logic.
- `WebServer.h/.cpp`: simple server model that works on one request for N cycles.
- `Request.h`: request data structure.
- `Config.h`: compile-time simulation constants.
- `Makefile`: build rules for the `loadbalancer` executable.
