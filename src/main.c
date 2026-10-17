
#include <stdio.h>
#include <unistd.h>

int read_cpu_stats(unsigned long long *idle,
                   unsigned long long *total) {
    FILE *fp = fopen("/proc/stat", "r");

    if (fp == NULL) {
        perror("Unable to open /proc/stat");
        return 0;
    }

    char label[10];
    unsigned long long user, nice, system, idle_time;
    unsigned long long iowait, irq, softirq, steal;
    unsigned long long guest, guest_nice;

    int count = fscanf(fp,
        "%9s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
        label, &user, &nice, &system, &idle_time,
        &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

    fclose(fp);

    if (count < 5) {
        fprintf(stderr, "Unable to read CPU statistics.\n");
        return 0;
    }

    *idle = idle_time + iowait;
    *total = user + nice + system + idle_time +
             iowait + irq + softirq + steal;

    return 1;
}

int main(void) {
    unsigned long long idle1, total1;
    unsigned long long idle2, total2;

    printf("====================================\n");
    printf("   SYSTEM RESOURCE ALERT SYSTEM\n");
    printf("        CPU MONITOR MODULE\n");
    printf("====================================\n");

    if (!read_cpu_stats(&idle1, &total1)) {
        return 1;
    }

    printf("Measuring CPU usage...\n");
    sleep(1);

    if (!read_cpu_stats(&idle2, &total2)) {
        return 1;
    }

    unsigned long long total_diff = total2 - total1;
    unsigned long long idle_diff = idle2 - idle1;

    if (total_diff == 0) {
        printf("Unable to calculate CPU usage.\n");
        return 1;
    }

    double cpu_usage =
        100.0 * (total_diff - idle_diff) / total_diff;

    printf("CPU Usage: %.2f%%\n", cpu_usage);
    printf("CPU monitoring completed successfully.\n");

    return 0;
}
