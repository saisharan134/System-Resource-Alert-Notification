
#include <stdio.h>
#include <string.h>

int main(void) {
    FILE *fp = fopen("/proc/meminfo", "r");

    if (fp == NULL) {
        perror("Unable to open /proc/meminfo");
        return 1;
    }

    char line[256];
    unsigned long total = 0;
    unsigned long available = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %lu kB", &total) == 1) {
            continue;
        }

        if (sscanf(line, "MemAvailable: %lu kB", &available) == 1) {
            break;
        }
    }

    fclose(fp);

    if (total == 0 || available > total) {
        fprintf(stderr, "Invalid memory statistics.\n");
        return 1;
    }

    unsigned long used = total - available;
    double usage = 100.0 * used / total;

    printf("====================================\n");
    printf("   SYSTEM RESOURCE ALERT SYSTEM\n");
    printf("       MEMORY MONITOR MODULE\n");
    printf("====================================\n");

    printf("Total Memory     : %.2f MB\n", total / 1024.0);
    printf("Available Memory : %.2f MB\n", available / 1024.0);
    printf("Used Memory      : %.2f MB\n", used / 1024.0);
    printf("Memory Usage     : %.2f%%\n", usage);

    if (usage >= 80.0) {
        printf("ALERT: High memory usage!\n");
    } else {
        printf("STATUS: Memory usage is within limits.\n");
    }

    return 0;
}
