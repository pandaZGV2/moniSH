#include "../global.h"

deque *init_hist(deque *dq)
{
    dq = (deque *)malloc(sizeof(deque));
    assert(dq);
    dq->head = NULL;
    dq->tail = NULL;
    dq->size = 0;
    char filename[1000];
    strcpy(filename, home_folder);
    strcat(filename, "/");
    strcat(filename, "history.log");
    FILE *fd = fopen(filename, "r");
    if (fd == NULL)
    {
        // perror("Failed to open history file");
        return dq;
    }
    char *read_cmd = (char *)malloc(sizeof(char) * 4096);
    char *newline_occurence = 0;
    while (fgets(read_cmd, 4096, fd) > 0)
    {
        if (strlen(read_cmd) <= 1)
        {
            continue;
        }
        if ((newline_occurence = strchr(read_cmd, '\n')) != NULL)
        {
            *newline_occurence = '\0';
        }
        deque_pushfront(dq, read_cmd);
    }
    free(read_cmd);
    fclose(fd);
    return dq;
}

void delete_history(deque *dq)
{
    node_hist *tmp = dq->tail;
    node_hist *prev_tmp = tmp;
    char filename[1000];
    strcpy(filename, home_folder);
    strcat(filename, "/");
    strcat(filename, "history.log");
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("Failed to open history");
    }
    char write_cmd[2000];
    while (tmp != NULL)
    {
        prev_tmp = tmp;
        tmp = tmp->prev;
        sprintf(write_cmd, "%s\n", prev_tmp->element);
        write(fd, write_cmd, strlen(write_cmd));
        free(prev_tmp);
    }
    close(fd);
    free(dq);
    dq = NULL;
}

void deque_pushfront(deque *d, char *element)
{
    if (d != NULL && d->head != NULL)
    {
        if (strcmp(element, d->head->element) == 0)
        {
            return;
        }
    }
    if (strlen(element) == 1 && element[0] == '\n')
    {
        return;
    }
    node_hist *new = malloc(sizeof(node_hist));
    assert(new);
    strcpy(new->element, element);
    new->next = d->head;
    new->prev = NULL;
    if (d->tail == NULL)
    {
        d->head = d->tail = new;
    }
    else
    {
        d->head->prev = new;
        d->head = new;
    }
    d->size++;
    if (d->size > 20)
    {
        char *elem = deque_popback(d);
        assert(elem);
        free(elem);
    }
}

void deque_pushback(deque *d, char *v)
{
    node_hist *new = malloc(sizeof(node_hist));
    assert(new);
    strcpy(new->element, v);
    new->prev = d->tail;
    new->next = NULL;
    if (d->head == NULL)
    {
        d->head = d->tail = new;
    }
    else
    {
        d->tail->next = new;
        d->tail = new;
    }
    d->size++;
}

char *deque_popfront(deque *d)
{
    char *elem = strdup(d->head->element);
    assert(elem);
    node_hist *n = d->head;
    if (d->head == d->tail)
        d->head = d->tail = NULL;
    else
        d->head = n->next;
    free(n);
    d->size--;
    return elem;
}

char *deque_popback(deque *d)
{
    char *elem = strdup(d->tail->element);
    assert(elem);
    node_hist *n = d->tail;
    if (d->head == d->tail)
        d->head = d->tail = NULL;
    else
        d->tail = n->prev;
    free(n);
    d->size--;
    return elem;
}

void history(deque *dq, int *n)
{
    int num = 0;
    if (*n > 10)
    {
        *n = 10;
    }
    if (*n == 0)
    {
        return;
    }
    char *commands_list[*n];
    node_hist *tmp = dq->head;
    while ((*n)-- > 0 && tmp != NULL)
    {
        commands_list[num] = tmp->element;
        tmp = tmp->next;
        num++;
    }
    for (int i = num - 1; i >= 0; i--)
    {
        printf("%s\n", commands_list[i]);
    }
}