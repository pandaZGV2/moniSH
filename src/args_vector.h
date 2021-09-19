#ifndef OS_ASSIGNMENT_2_ARGS_VECTOR_H
#define OS_ASSIGNMENT_2_ARGS_VECTOR_H

typedef struct
{
    char **argv;
    int size;
    int used;
} args_vector;

#define INIT_SIZE 5

void init_args_vector(args_vector *argv);
void expand_argv(args_vector *argv);
void free_argv(args_vector *argv);
void add_arg(args_vector *argv, char *token);
args_vector *convert_to_argv(char *cmd);
char *make_command(args_vector *argv, int start_index);

#endif //OS_ASSIGNMENT_2_ARGS_VECTOR_H
