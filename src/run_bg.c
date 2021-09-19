#include "../global.h"

void run_bg(args_vector *argv)
{
    free(argv->argv[--(argv->used)]);
    pid_t child;
    child = fork();
    if (child == -1)
    {
        perror("Error using fork()\n");
        return;
    }
    else if (child == 0)
    {
        int status = execvp(argv->argv[0], argv->argv);
        if (status < 0)
        {
            perror("Error using execvp()");
            exit(1);
        }
    }
    else
    {
        setpgid(child, 0);
        tcsetpgrp(0, getpgrp());
        printf("%d\n", child);
        add_proc(bg_list, child, argv->argv[0]);
    }
}
