#include "proclore.h"


pid_t get_process_group(pid_t pid) {
    char stat_file[256];
    snprintf(stat_file, sizeof(stat_file), "/proc/%d/stat", pid);

    FILE *file = fopen(stat_file, "r");
    if (!file) {
        perror("Failed to open stat file");
        exit(1);
    }

    int pgid;
    fscanf(file, "%*d %*s %*c %*d %*d %d", &pgid);
    fclose(file);

    return pgid;
}

void proclore(int pid,char* home) {
    char path[40], line[100];
    FILE *statusf;

    sprintf(path, "/proc/%d/status", pid);
    statusf = fopen(path, "r");
    if (!statusf) {
        perror("Unable to open /proc/<pid>/status");
        return;
    }

    char state[3] = "";
    int process_group = -1;
    int vm_size = -1;

    while (fgets(line, 100, statusf)) {
        if (strncmp(line, "State:", 6) == 0) {
            sscanf(line, "State: %s", state);
        } else if (strncmp(line, "Tgid:", 5) == 0) {
            sscanf(line, "Tgid: %d", &process_group);
        } else if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line, "VmSize: %d", &vm_size);
        }
    }
    fclose(statusf);

    char exe_path[1024];
    sprintf(path, "/proc/%d/exe", pid);
    int len = readlink(path, exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("Unable to read /proc/<pid>/exe");
        return;
    }
    exe_path[len] = '\0';
    
    if(tcgetpgrp(STDIN_FILENO) == getpgid(pid)){
        strcat(state,"+");
    }

    printf("PID: %d\n", pid);
    printf("Process Status: %s\n", state);
    printf("Process Group: %d\n", process_group);
    printf("Virtual Memory: %d KB\n", vm_size);

    strcpy(exe_path,relative_path(home,exe_path));
    printf("Executable Path: %s\n", exe_path);
}

