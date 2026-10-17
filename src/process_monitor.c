
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

int main(void) {
    DIR *proc = opendir("/proc");

    if (proc == NULL) {
        perror("Unable to open /proc");
        return 1;
    }

    struct dirent *entry;
    int count = 0;

    printf("================================================\n");
    printf("       SYSTEM RESOURCE ALERT SYSTEM\n");
    printf("             PROCESS MONITOR\n");
    printf("================================================\n");
    printf("%-10s %-25s %12s\n", "PID", "PROCESS NAME", "MEMORY (KB)");
    printf("------------------------------------------------\n");

    while ((entry = readdir(proc)) != NULL) {
        if (!isdigit((unsigned char)entry->d_name[0])) {
            continue;
        }

        char path[256];

        int written = snprintf(path, sizeof(path),
                               "/proc/%s/status", entry->d_name);

        if (written < 0 || (size_t)written >= sizeof(path)) {
            continue;
        }

        FILE *fp = fopen(path, "r");

        if (fp == NULL) {
            continue;
        }

        char line[256];
        char name[128] = "unknown";
        unsigned long memory_kb = 0;

        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "Name:", 5) == 0) {
                sscanf(line + 5, "%127s", name);
            } else if (strncmp(line, "VmRSS:", 6) == 0) {
                sscanf(line + 6, "%lu", &memory_kb);
            }
        }

        fclose(fp);

        printf("%-10s %-25.25s %12lu\n",
               entry->d_name, name, memory_kb);

        count++;
    }

    closedir(proc);

    printf("------------------------------------------------\n");
    printf("Processes inspected: %d\n", count);
    printf("Process monitoring completed.\n");

    return 0;
}
