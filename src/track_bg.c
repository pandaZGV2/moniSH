#include "../global.h"

void track_bg()
{
    int wstatus = 0;
    pid_t child = 0;
    char *stat = (char *)calloc(500, sizeof(char));
    while ((child = waitpid(-1, &wstatus, WNOHANG | WUNTRACED)) > 0)
    {
        char *proc_name = get_proc_name(bg_list, child);
        if (WIFEXITED(wstatus))
        {
            sprintf(stat, "%s with pid %d exited normally\n", proc_name, child);
            // fprintf(stderr, "%s with pid %d exited normally\n", proc_name, child);
            delete_proc(bg_list, child, proc_name);
            push(status_list, stat);
        }
        else if (WIFSIGNALED(wstatus))
        {
            sprintf(stat, "%s with pid %d exited abnormally\n", proc_name, child);
            //            fprintf(stderr, "%s with pid %d exited abnormally\n", proc_name, child);
            delete_proc(bg_list, child, proc_name);
            push(status_list, stat);
        }
    }
    free(stat);
}