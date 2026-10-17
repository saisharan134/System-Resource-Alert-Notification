#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <sys/statvfs.h>

#define CONFIG_FILE "config/config.conf"

double cpu_limit = 80.0;
double memory_limit = 80.0;
double disk_limit = 80.0;
int monitor_interval = 5;

void load_config(void) {
    FILE *fp = fopen(CONFIG_FILE, "r");

    if (!fp) {
        printf("Config unavailable; using defaults.\n");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        char key[100], value[100], extra;
        char *p = line;

        while (isspace((unsigned char)*p))
            p++;

        if (*p == '#' || *p == '\0')
            continue;

        if (sscanf(p, " %99[^=]=%99s %c",
                   key, value, &extra) < 2)
            continue;

        char *k = key + strlen(key);
        while (k > key && isspace((unsigned char)k[-1]))
            *--k = '\0';

        char *end;
        double number = strtod(value, &end);

        if (*end != '\0')
            continue;

        if (strcmp(key, "CPU_THRESHOLD") == 0 &&
            number >= 0 && number <= 100) {
            cpu_limit = number;
        } else if (strcmp(key, "MEMORY_THRESHOLD") == 0 &&
                   number >= 0 && number <= 100) {
            memory_limit = number;
        } else if (strcmp(key, "DISK_THRESHOLD") == 0 &&
                   number >= 0 && number <= 100) {
            disk_limit = number;
        } else if (strcmp(key, "MONITOR_INTERVAL") == 0 &&
                   number >= 1 && number <= 3600 &&
                   number == (int)number) {
            monitor_interval = (int)number;
        }
    }

    fclose(fp);
}

int read_cpu(unsigned long long *idle,
             unsigned long long *total) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp)
        return 0;

    char label[10];
    unsigned long long v[8] = {0};

    int n = fscanf(fp,
        "%9s %llu %llu %llu %llu %llu %llu %llu %llu",
        label, &v[0], &v[1], &v[2], &v[3],
        &v[4], &v[5], &v[6], &v[7]);

    fclose(fp);

    if (n < 5)
        return 0;

    *idle = v[3] + v[4];
    *total = v[0] + v[1] + v[2] + v[3] +
             v[4] + v[5] + v[6] + v[7];

    return 1;
}

int cpu_usage(double *usage) {
    unsigned long long i1, t1, i2, t2;

    if (!read_cpu(&i1, &t1))
        return 0;

    sleep(1);

    if (!read_cpu(&i2, &t2))
        return 0;

    if (t2 <= t1 || i2 < i1)
        return 0;

    *usage = 100.0 * ((t2 - t1) - (i2 - i1))
             / (t2 - t1);

    return 1;
}

int memory_usage(double *usage) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp)
        return 0;

    char line[256];
    unsigned long total = 0, available = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %lu kB", &total) == 1)
            continue;

        if (sscanf(line, "MemAvailable: %lu kB",
                   &available) == 1)
            break;
    }

    fclose(fp);

    if (total == 0 || available > total)
        return 0;

    *usage = 100.0 * (total - available) / total;
    return 1;
}

int disk_usage(double *usage) {
    struct statvfs s;

    if (statvfs("/", &s) != 0)
        return 0;

    unsigned long long total =
        (unsigned long long)s.f_blocks * s.f_frsize;

    unsigned long long available =
        (unsigned long long)s.f_bavail * s.f_frsize;

    if (total == 0 || available > total)
        return 0;

    *usage = 100.0 * (total - available) / total;
    return 1;
}

void check_alert(const char *name,
                 double usage, double limit) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[32] = "unknown";

    if (tm_info)
        strftime(timestamp, sizeof(timestamp),
                 "%Y-%m-%d %H:%M:%S", tm_info);

    printf("%s usage: %.2f%% | Threshold: %.2f%%\n",
           name, usage, limit);

    if (usage >= limit) {
        printf("ALERT: High %s usage detected!\n", name);

        FILE *log = fopen("logs/alerts.log", "a");

        if (log) {
            fprintf(log,
                "[%s] ALERT: %s usage %.2f%% "
                "(threshold %.2f%%)\n",
                timestamp, name, usage, limit);
            fclose(log);
        } else {
            perror("Unable to write alert log");
        }
    } else {
        printf("STATUS: %s usage is within limits.\n", name);
    }
}

int main(int argc, char *argv[]) {
    int continuous = argc > 1 &&
        strcmp(argv[1], "--continuous") == 0;

    load_config();

    printf("SYSTEM RESOURCE ALERT AND NOTIFICATION SYSTEM\n");
    printf("CPU threshold: %.1f%%\n", cpu_limit);
    printf("Memory threshold: %.1f%%\n", memory_limit);
    printf("Disk threshold: %.1f%%\n", disk_limit);
    printf("Monitoring interval: %d seconds\n",
           monitor_interval);

    do {
        double value;

        printf("\n--- Resource Check ---\n");

        if (cpu_usage(&value))
            check_alert("CPU", value, cpu_limit);
        else
            printf("ERROR: Unable to read CPU usage.\n");

        if (memory_usage(&value))
            check_alert("Memory", value, memory_limit);
        else
            printf("ERROR: Unable to read memory usage.\n");

        if (disk_usage(&value))
            check_alert("Disk", value, disk_limit);
        else
            printf("ERROR: Unable to read disk usage.\n");

        if (continuous) {
            printf("Next check in %d seconds. "
                   "Press Ctrl+C to stop.\n",
                   monitor_interval);
            sleep((unsigned int)monitor_interval);
        }

    } while (continuous);

    printf("\nMonitoring completed.\n");
    return 0;
}
