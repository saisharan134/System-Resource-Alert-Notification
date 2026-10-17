# System Resource Alert and Notification System

## 1. Project Overview

The System Resource Alert and Notification System is a Linux-based command-line application developed in C for the OSSP Hackathon. It monitors CPU utilization, memory consumption, and disk usage, identifies resource usage that exceeds predefined thresholds, and records alerts with timestamps.

The system also includes a process-monitoring module that lists running processes, their process IDs (PIDs), and resident memory usage.

## 2. Problem Statement

Excessive CPU usage, memory consumption, and disk utilization can affect system performance and stability. This project provides a lightweight monitoring solution that measures resource utilization and generates alerts when configured limits are exceeded.

## 3. Objectives

- Monitor CPU utilization using Linux system statistics.
- Measure memory utilization using `/proc/meminfo`.
- Monitor disk usage using the `statvfs()` system call.
- List running processes and their memory usage.
- Generate threshold-based alerts.
- Store alert history in a log file.
- Demonstrate Operating Systems concepts using C and Linux interfaces.

## 4. Modules

1. **CPU Monitoring:** Reads `/proc/stat` and calculates CPU utilization between two samples.
2. **Memory Monitoring:** Reads total and available memory from `/proc/meminfo`.
3. **Disk Monitoring:** Uses `statvfs()` to calculate disk space utilization for the root filesystem.
4. **Process Monitoring:** Reads process information from `/proc` and displays PID, process name, and resident memory.
5. **Alert Engine:** Compares supplied resource percentages against thresholds and logs violations.
6. **Integrated Monitoring:** Collects actual CPU, memory, and disk statistics, checks thresholds, and supports repeated monitoring.

## 5. Technologies Used

- Programming language: C
- Operating system: Linux / Ubuntu
- Compiler: GCC
- Version control: Git
- Repository hosting: GitHub
- Linux interfaces: `/proc/stat`, `/proc/meminfo`, `/proc/[PID]/status`
- System call/library interface: `statvfs()`

## 6. Project Structure

```text
SystemResourceAlert/
├── src/
│   ├── main.c
│   ├── memory_monitor.c
│   ├── disk_monitor.c
│   ├── process_monitor.c
│   └── alert_engine.c
├── include/
│   └── monitor.h
├── config/
│   └── config.conf
├── logs/
├── tests/
├── docs/
├── reports/
├── README.md
└── .gitignore
```

The CPU-monitoring implementation is included in `src/main.c` as part of the integrated monitoring program. The standalone `cpu_monitor` executable is generated during compilation and is not committed to Git.

## 7. Prerequisites

- Ubuntu or another compatible Linux environment.
- GCC compiler.
- Git.
- A terminal with access to Linux `/proc` and filesystem statistics.

Install the compiler and Git if needed:

```bash
sudo apt update
sudo apt install build-essential git
```

## 8. Setup

Clone the repository:

```bash
git clone https://github.com/saisharan134/System-Resource-Alert-Notification.git
cd System-Resource-Alert-Notification
```

If you are continuing with an existing local copy, open the `SystemResourceAlert` project directory instead.

## 9. Compilation and Execution

Compile the integrated monitoring system:

```bash
gcc -Wall -Wextra -o system_monitor src/main.c
```

Run a single monitoring cycle:

```bash
./system_monitor
```

Run continuous monitoring:

```bash
./system_monitor --continuous
```

Press `Ctrl+C` to stop continuous monitoring.

Compile and run the individual modules:

```bash
gcc -Wall -Wextra -o memory_monitor src/memory_monitor.c
./memory_monitor

gcc -Wall -Wextra -o disk_monitor src/disk_monitor.c
./disk_monitor

gcc -Wall -Wextra -o process_monitor src/process_monitor.c
./process_monitor

gcc -Wall -Wextra -o alert_engine src/alert_engine.c
./alert_engine
```

The standalone CPU implementation is integrated into `src/main.c`; compile the integrated program to test it.

## 10. Alert Thresholds

The integrated monitoring program currently uses these default thresholds:

| Resource | Threshold |
|---|---:|
| CPU | 80% |
| Memory | 80% |
| Disk | 80% |

When a resource reaches or exceeds its threshold, the program prints an alert and appends a timestamped entry to `logs/alerts.log`.

The standalone alert engine accepts resource percentages interactively for testing. Threshold configuration through `config/config.conf` is a planned enhancement.

## 11. Testing

The project has been manually tested for:

- Successful compilation with GCC.
- CPU usage measurement under a temporary CPU load.
- Memory and disk statistics retrieval.
- Running-process enumeration.
- Alert generation when test values exceed the threshold.
- Timestamped alert logging.
- Single-cycle and continuous monitoring modes.
- Git commits and repository synchronization.

Record final test inputs, observed outputs, and screenshots in the `tests/` and `reports/` directories before submission.

## 12. Limitations and Future Enhancements

- Add runtime configuration for thresholds through `config/config.conf`.
- Add configurable monitoring intervals.
- Improve alert deduplication and severity classification.
- Add more detailed process CPU utilization measurements.
- Expand automated test coverage.
- Add optional email or desktop notifications if permitted by the evaluation environment.

The current implementation is a terminal-based prototype; it does not include a graphical user interface.

## 13. Team

**Hackathon title:** System Resource Alert and Notification System

**Team number:** G-13

**Team members:**
- Kolavali Sanjana
- Kompelli Saanvi
- Sai Sharan Rangaraj Cholkar

**Supervisor:** Add your faculty supervisor's name before submission.

## 14. Current Status

The initial monitoring modules and integrated command-line prototype have been implemented. Testing, configuration enhancements, documentation, and final presentation preparation are in progress.

## 15. Repository

GitHub: https://github.com/saisharan134/System-Resource-Alert-Notification
