#ifndef OS_ASSIGNMENT_2_HISTORY_H
#define OS_ASSIGNMENT_2_HISTORY_H

typedef struct node_hist node_hist;

struct node_hist
{
    node_hist *next;
    node_hist *prev;
    char element[1000];
};

typedef struct deque deque;

struct deque
{
    node_hist *head;
    node_hist *tail;
    int size;
};

deque *init_hist(deque *dq);
void delete_history(deque *dq);
void deque_pushfront(deque *d, char *element);
void deque_pushback(deque *d, char *v);
char *deque_popfront(deque *d);
char *deque_popback(deque *d);
void history(deque *dq, int *n);

#endif //OS_ASSIGNMENT_2_HISTORY_H
