
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/statvfs.h>

#define CPU_LIMIT 80.0
#define MEMORY_LIMIT 80.0
#define DISK_LIMIT 80.0

int read_cpu(unsigned long long *idle,
             unsigned long long *total) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return 0;

    char label[10];
    unsigned long long v[10] = {0};

    int n = fscanf(fp,
        "%9s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
        label, &v[0], &v[1], &v[2], &v[3], &v[4],
        &v[5], &v[6], &v[7], &v[8], &v[9]);

    fclose(fp);
    if (n < 5) return 0;

    *idle = v[3] + v[4];
    *total = v[0] + v[1] + v[2] + v[3] + v[4]
             + v[5] + v[6] + v[7];
    return 1;
}

int cpu_usage(double *usage) {
    unsigned long long i1, t1, i2, t2;

    if (!read_cpu(&i1, &t1)) return 0;
    sleep(1);
    if (!read_cpu(&i2, &t2)) return 0;

    if (t2 <= t1 || i2 < i1) return 0;

    unsigned long long td = t2 - t1;
    unsigned long long id = i2 - i1;

    *usage = 100.0 * (td - id) / td;
    return 1;
}

int memory_usage(double *usage) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return 0;

    char line[256];
    unsigned long total = 0, available = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %lu kB", &total) == 1)
            continue;
        if (sscanf(line, "MemAvailable: %lu kB", &available) == 1)
            break;
    }

    fclose(fp);

    if (total == 0 || available > total) return 0;
    *usage = 100.0 * (total - available) / total;
    return 1;
}

int disk_usage(double *usage) {
    struct statvfs s;

    if (statvfs("/", &s) != 0) return 0;

    unsigned long long total =
        (unsigned long long)s.f_blocks * s.f_frsize;
    unsigned long long available =
        (unsigned long long)s.f_bavail * s.f_frsize;

    if (total == 0 || available > total) return 0;

    *usage = 100.0 * (total - available) / total;
    return 1;
}

void check_alert(const char *name, double usage, double limit) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[32] = "unknown";

    if (tm_info)
        strftime(timestamp, sizeof(timestamp),
                 "%Y-%m-%d %H:%M:%S", tm_info);

    printf("%s usage: %.2f%% | Limit: %.2f%%\n",
           name, usage, limit);

    if (usage >= limit) {
        printf("ALERT: High %s usage!\n", name);

        FILE *log = fopen("logs/alerts.log", "a");
        if (log) {
            fprintf(log, "[%s] ALERT: %s usage %.2f%% "
                         "(limit %.2f%%)\n",
                    timestamp, name, usage, limit);
            fclose(log);
        } else {
            perror("Unable to write alert log");
        }
    }
}

int main(int argc, char *argv[]) {
    int continuous = argc > 1 &&
                     strcmp(argv[1], "--continuous") == 0;

    do {
        double cpu, memory, disk;

        printf("\n====================================\n");
        printf(" SYSTEM RESOURCE ALERT SYSTEM\n");
        printf("====================================\n");

        if (cpu_usage(&cpu))
            check_alert("CPU", cpu, CPU_LIMIT);
        else
            printf("ERROR: Unable to read CPU usage.\n");

        if (memory_usage(&memory))
            check_alert("Memory", memory, MEMORY_LIMIT);
        else
            printf("ERROR: Unable to read memory usage.\n");

        if (disk_usage(&disk))
            check_alert("Disk", disk, DISK_LIMIT);
        else
            printf("ERROR: Unable to read disk usage.\n");

        if (continuous) {
            printf("Next check in 5 seconds. Press Ctrl+C to stop.\n");
            sleep(5);
        }
    } while (continuous);

    printf("Monitoring completed.\n");
    return 0;
}
