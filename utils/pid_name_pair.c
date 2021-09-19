#include "../global.h"

#define INIT_SIZE 5

proc_list *init_proclist(proc_list *list)
{
    list = (proc_list *)malloc(sizeof(proc_list));
    assert(list);
    list->proc_list = NULL;
    list->used = 0;

    return list;
}

void free_proclist(proc_list *list)
{
    pair *curr = list->proc_list;
    pair *prev = curr;
    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next;
        if (prev->name != NULL)
        {
            free(prev->name);
        }
        free(prev);
    }
    free(list);
}

void add_proc(proc_list *list, pid_t id, char *name)
{
    pair *new = (pair *)malloc(sizeof(pair));
    new->next = NULL;
    new->name = strdup(name);
    new->id = id;
    new->next = list->proc_list;
    list->proc_list = new;
    list->used++;
}

char *get_proc_name(proc_list *list, pid_t id)
{
    pair *curr = list->proc_list;
    while (curr != NULL)
    {
        if (curr->id == id)
        {
            return curr->name;
        }
        curr = curr->next;
    }
    return NULL;
}

void delete_proc(proc_list *list, pid_t id, char *name)
{
    pair *curr = list->proc_list;
    pair *prev = curr;
    if (curr == NULL)
    {
        return;
    }
    else if (curr->id == id)
    {
        list->proc_list = curr->next;
        list->used--;
        if (curr->name)
        {
            free(curr->name);
        }
        free(curr);
        return;
    }
    while (curr != NULL && curr->id != id)
    {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL)
    {
        return;
    }
    else
    {
        prev->next = curr->next;
        if (curr->name)
        {
            free(curr->name);
        }
        free(curr);
        list->used--;
    }
}