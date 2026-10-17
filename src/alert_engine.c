
#include <stdio.h>
#include <time.h>

void check_resource(const char *name, double usage, double threshold) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char timestamp[32] = "unknown";

    if (local != NULL) {
        strftime(timestamp, sizeof(timestamp),
                 "%Y-%m-%d %H:%M:%S", local);
    }

    printf("%s usage: %.2f%% | Threshold: %.2f%%\n",
           name, usage, threshold);

    if (usage >= threshold) {
        printf("ALERT: High %s usage detected!\n", name);

        FILE *log = fopen("logs/alerts.log", "a");
        if (log != NULL) {
            fprintf(log, "[%s] ALERT: High %s usage: %.2f%% "
                         "(threshold: %.2f%%)\n",
                    timestamp, name, usage, threshold);
            fclose(log);
        } else {
            perror("Unable to write alert log");
        }
    } else {
        printf("STATUS: %s usage is within limits.\n", name);
    }
}

int main(void) {
    double cpu, memory, disk;

    printf("====================================\n");
    printf("   SYSTEM RESOURCE ALERT SYSTEM\n");
    printf("          ALERT ENGINE\n");
    printf("====================================\n");

    printf("Enter CPU usage percentage: ");
    if (scanf("%lf", &cpu) != 1 || cpu < 0 || cpu > 100) {
        fprintf(stderr, "Invalid CPU percentage.\n");
        return 1;
    }

    printf("Enter memory usage percentage: ");
    if (scanf("%lf", &memory) != 1 || memory < 0 || memory > 100) {
        fprintf(stderr, "Invalid memory percentage.\n");
        return 1;
    }

    printf("Enter disk usage percentage: ");
    if (scanf("%lf", &disk) != 1 || disk < 0 || disk > 100) {
        fprintf(stderr, "Invalid disk percentage.\n");
        return 1;
    }

    printf("\n--- Alert Results ---\n");
    check_resource("CPU", cpu, 80.0);
    check_resource("Memory", memory, 80.0);
    check_resource("Disk", disk, 80.0);

    return 0;
}
