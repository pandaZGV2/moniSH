#include "../global.h"

void pinfo(char *pid)
{
    char process_id[100];
    if (pid == NULL)
    {
        sprintf(process_id, "%d", getpid());
    }
    else
    {
        strcpy(process_id, pid);
    }
    printf("pid -- %s\n", process_id);

    char stat_file[200];
    sprintf(stat_file, "/proc/%s/stat", process_id);
    char *buffer = (char *)calloc(1000001, sizeof(char));
    int fd = open(stat_file, O_RDONLY);
    if (fd < 0)
    {
        perror("Could not open stat file");
        free(buffer);
        return;
    }
    if (read(fd, buffer, 1000000) == 0)
    {
        printf("Could not read from %s\n", stat_file);
        free(buffer);
        return;
    }
    char *token = NULL;
    char *saveptr = NULL;
    token = strtok_r(buffer, " ", &saveptr);
    token = strtok_r(NULL, " ", &saveptr);
    token = strtok_r(NULL, " ", &saveptr);
    close(fd);
    printf("Process Status -- %s", token);
    token = strtok_r(NULL, " ", &saveptr);
    token = strtok_r(NULL, " ", &saveptr);
    int pgrp = atoi(token);
    token = strtok_r(NULL, " ", &saveptr);
    token = strtok_r(NULL, " ", &saveptr);
    token = strtok_r(NULL, " ", &saveptr);
    int tpgid = atoi(token);
    if (tpgid == pgrp)
    {
        printf("+\n");
    }
    else
    {
        printf("\n");
    }
    for(int i = 9; i <= 23; i++)
    {
        token = strtok_r(NULL, " ", &saveptr);
    }

    // char statm_file[200];
    // sprintf(statm_file, "/proc/%s/statm", process_id);
    // fd = open(statm_file, O_RDONLY);
    // if (fd < 0)
    // {
    //     perror("Could not open stat file");
    //     free(buffer);
    //     return;
    // }
    // if (read(fd, buffer, 1000000) == 0)
    // {
    //     printf("Could not read from %s\n", statm_file);
    //     free(buffer);
    //     return;
    // }
    // saveptr = NULL;
    // token = strtok_r(buffer, " ", &saveptr);
    printf("memory -- %s\n", token);
    close(fd);
    free(buffer);

    char *buf_exec = (char *)calloc(10001, sizeof(char));
    char exe_file[200];
    if (pid == NULL)
    {
        sprintf(exe_file, "/proc/%s/exe", process_id);
        if (readlink(exe_file, buf_exec, 100001) <= 0)
        {
            char error[500];
            sprintf(error, "Cannot read from %s", exe_file);
            perror(error);
            free(buf_exec);
            return;
        }
        char *saveptr1 = NULL;
        char *token1 = strtok_r(buf_exec, "/", &saveptr1);
        char *exec;
        while (token1 != NULL)
        {
            exec = token1;
            token1 = strtok_r(NULL, "/", &saveptr1);
        }
        printf("Executable Path -- ~/%s\n", exec);
    }
    else
    {
        sprintf(exe_file, "/proc/%s/exe", process_id);
        if (readlink(exe_file, buf_exec, 1000001) <= 0)
        {
            char error[500];
            sprintf(error, "Cannot read from %s", exe_file);
            perror(error);
            free(buf_exec);
            return;
        }
        printf("Executable Path -- %s\n", buf_exec);
    }
    free(buf_exec);
}