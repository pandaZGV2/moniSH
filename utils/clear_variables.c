#include "../global.h"

extern char *prompt;
void clear_vars()
{
    free(username);
    free(prompt);
    free(cmd);
    free(hostname);
    free(previous_directory);
    free_proclist(bg_list);
    destroystack(status_list);
    delete_history(hist);
    free(home_folder);
}