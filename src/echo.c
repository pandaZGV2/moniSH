#include "../global.h"

void echo(args_vector *argv)
{
    for (int i = 1; i < argv->used - 1; i++)
    {
        printf("%s ", argv->argv[i]);
    }
    if (argv->used > 1)
    {
        printf("%s", argv->argv[argv->used - 1]);
    }
    printf("\n");
}
