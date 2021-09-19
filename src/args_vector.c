#include "../global.h"

void init_args_vector(args_vector *argv)
{
    argv->argv = (char **)calloc(sizeof(char *), (INIT_SIZE));
    argv->size = INIT_SIZE;
    argv->used = 0;
}
void argv_resize(args_vector *argv)
{
    argv->size = argv->size * 2;
    argv->argv = (char **)realloc(argv->argv, sizeof(char *) * argv->size);
    if (argv->argv == NULL)
    {
        printf("Error in realloc\n");
        exit(1);
    }
    for (int i = argv->used; i < argv->size; i++)
    {
        argv->argv[i] = NULL;
    }
}
void free_argv(args_vector *argv)
{
    for (int i = 0; i < argv->size; i++)
    {
        free(argv->argv[i]);
    }
    free(argv->argv);
    free(argv);
}
void add_arg(args_vector *argv, char *token)
{
    if (argv->used == argv->size)
    {
        argv_resize(argv);
    }
    argv->argv[argv->used] = (char *)malloc(strlen(token) + 1);
    strcpy(argv->argv[argv->used], token);
    argv->argv[argv->used][strlen(token)] = 0;
    argv->used++;
}
char *make_command(args_vector *argv, int start_index)
{
    char *str = (char *)calloc(1000, sizeof(char));
    assert(str);
    for (int i = start_index; i < argv->used; i++)
    {
        strcat(str, argv->argv[i]);
        strcat(str, " ");
    }
    return str;
}
args_vector *convert_to_argv(char *command)
{
    args_vector *argv = (args_vector *)malloc(sizeof(args_vector));
    char *str = strdup(command);
    if (argv == NULL)
    {
        printf("Error in malloc\n");
        exit(1);
    }
    init_args_vector(argv);

    char *token = NULL;
    char *saveptr = NULL;
    char delimit[] = " \t\n\f\r\v";

    token = strtok_r(str, delimit, &saveptr);
    while (token != NULL)
    {
        add_arg(argv, token);
        token = strtok_r(NULL, delimit, &saveptr);
    }
    free(str);
    return argv;
}