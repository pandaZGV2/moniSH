#include "../global.h"

int cd(char *token1)
{
    if (strcmp(token1, "-") == 0)
    {
        if (strlen(previous_directory) == 0)
        {
            printf("Previous Working Directory is not set yet.\n");
            return -2;
        }
        return chdir(previous_directory);
    }
    if (strcmp(token1, "~") == 0)
    {
        return chdir(home_folder);
    }
    char *real_dir = NULL;
    char *actual_path = replace_string(token1, "~", home_folder);
    real_dir = realpath(actual_path, NULL);
    if (errno == ENOENT)
    {
        perror("Directory does not exist");
        return -2;
    }
    assert(real_dir);
    int status = chdir(real_dir);
    free(real_dir);
    free(actual_path);
    return status;
}
