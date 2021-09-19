#ifndef OS_ASSIGNMENT_2_PID_NAME_PAIR_H
#define OS_ASSIGNMENT_2_PID_NAME_PAIR_H

#include <sys/types.h>

typedef struct id_name_pair pair;
struct id_name_pair
{
    char *name;
    pid_t id;
    pair *next;
};

typedef struct
{
    pair *proc_list;
    int used;
} proc_list;

extern proc_list *bg_list;

proc_list *init_proclist(proc_list *list);
void free_proclist(proc_list *list);
void add_proc(proc_list *list, pid_t id, char *name);
char *get_proc_name(proc_list *list, pid_t id);
void delete_proc(proc_list *list, pid_t id, char *name);

#endif //OS_ASSIGNMENT_2_PID_NAME_PAIR_H
