# System Resource Alert and Notification System
## OSSP Hackathon Project Report

### 1. Abstract
This project implements a lightweight Linux resource monitoring application in C.
It measures CPU utilization, memory consumption and disk usage, compares
measurements with configured thresholds, and records alerts in a log file.

### 2. Problem Statement
Excessive system resource consumption can affect performance. A lightweight
monitoring tool can identify resource usage that exceeds specified limits.

### 3. Objectives
- Monitor CPU, memory and disk utilization.
- Display resource statistics.
- Detect threshold violations.
- Record timestamped alerts.
- Inspect running processes.
- Demonstrate Linux and Operating Systems concepts.

### 4. Technologies
C, GCC, Ubuntu/Linux, Git and GitHub.

### 5. Implementation
CPU data is read from /proc/stat. Memory information is obtained from
/proc/meminfo. Disk statistics are measured with statvfs(). Process
information is read from /proc. The alert engine compares usage values
with configured limits and writes alerts to logs/alerts.log.

### 6. Testing
The integrated program has been executed to display actual resource
readings. Configuration loading and manually supplied alert scenarios
have been demonstrated. Further threshold and continuous-mode tests
should be recorded with their actual results.

### 7. Limitations
The prototype uses a terminal interface. Resource readings can fluctuate.
The integrated monitor and individual modules are separate programs;
not every standalone module is necessarily executed by the integrated program.

### 8. Future Scope
Add more robust error handling, alert deduplication, automated tests,
and additional notification methods.

### 9. Conclusion
The project demonstrates Linux resource monitoring, threshold-based alerts,
file handling, process inspection and practical Operating Systems concepts.
