# System Architecture

The application is a Linux command-line monitoring system written in C.

## Modules
1. CPU Monitor: Reads CPU counters from /proc/stat.
2. Memory Monitor: Reads RAM information from /proc/meminfo.
3. Disk Monitor: Uses statvfs() to measure filesystem utilization.
4. Process Monitor: Reads process information from /proc.
5. Alert Engine: Compares resource usage against configured thresholds.
6. Integrated Monitor: Collects resource readings, checks thresholds and logs alerts.

## Data Flow
Linux system information -> Resource measurement -> Threshold comparison
-> Console alert -> Timestamped log file

## Operating System Concepts
- Linux process and filesystem interfaces
- File handling
- System calls and library functions
- Resource utilization
- Process IDs and memory information
- Periodic monitoring

The prototype does not use a graphical interface or database.
