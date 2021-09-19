#include "../global.h"

void run_fg(args_vector *argv)
{
    argv->argv[argv->used] = NULL;
    pid_t child;
    child = fork();
    if (child == -1)
    {
        perror("Error using fork()\n");
        exit(1);
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
//        signal(SIGTTIN, SIG_IGN);
//        signal(SIGTTOU, SIG_IGN);
//        setpgid(child, 0);
//        tcsetpgrp(0, __getpgid(child));
        int status;
        waitpid(child, &status, WUNTRACED);
//        tcsetpgrp(0, getpgrp());
//        signal(SIGTTIN, SIG_DFL);
//        signal(SIGTTOU, SIG_DFL);
    }
}