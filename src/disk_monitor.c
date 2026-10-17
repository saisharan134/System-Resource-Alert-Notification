
#include <stdio.h>
#include <sys/statvfs.h>

int main(void) {
    struct statvfs disk;

    printf("====================================\n");
    printf("   SYSTEM RESOURCE ALERT SYSTEM\n");
    printf("         DISK MONITOR MODULE\n");
    printf("====================================\n");

    /* Check disk usage of the root filesystem */
    if (statvfs("/", &disk) != 0) {
        perror("Unable to read disk statistics");
        return 1;
    }

    unsigned long long total =
        (unsigned long long)disk.f_blocks * disk.f_frsize;

    unsigned long long available =
        (unsigned long long)disk.f_bavail * disk.f_frsize;

    if (total == 0 || available > total) {
        fprintf(stderr, "Invalid disk statistics.\n");
        return 1;
    }

    unsigned long long used = total - available;

    double total_gb = total / (1024.0 * 1024 * 1024);
    double used_gb = used / (1024.0 * 1024 * 1024);
    double available_gb = available / (1024.0 * 1024 * 1024);
    double usage = 100.0 * used / total;

    printf("Filesystem       : /\n");
    printf("Total Disk Space : %.2f GB\n", total_gb);
    printf("Used Disk Space  : %.2f GB\n", used_gb);
    printf("Available Space  : %.2f GB\n", available_gb);
    printf("Disk Usage       : %.2f%%\n", usage);

    if (usage >= 80.0) {
        printf("ALERT: High disk usage!\n");
    } else {
        printf("STATUS: Disk usage is within limits.\n");
    }

    return 0;
}
