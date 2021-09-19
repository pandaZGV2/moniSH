#ifndef OS_ASSIGNMENT2_STACK_H
#define OS_ASSIGNMENT2_STACK_H

typedef struct node node;
typedef struct stack stack;

struct node
{
    char element[1000];
    struct node *next;
};
struct stack
{
    int size;
    node *nptr;
};

extern stack *status_list;

stack *initstack();
node *makenode(char *element);
void push(stack *, char *);
void pop(stack *);
void destroystack(stack *);
char *top(stack *);
void print_stack(stack *);

#endif //OS_ASSIGNMENT2_STACK_H
