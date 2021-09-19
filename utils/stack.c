#include "../global.h"

stack *initstack()
{
    stack *new_stack = (stack *)malloc(sizeof(stack));
    new_stack->size = 0;
    new_stack->nptr = NULL;

    return new_stack;
}
node *makenode(char *element)
{
    node *new_node = (node *)calloc(1, sizeof(node));
    strcpy(new_node->element, element);

    new_node->next = NULL;

    return new_node;
}

void push(stack *s, char *element)
{
    node *new_node = makenode(element);

    if (s->nptr == NULL)
    {
        s->nptr = new_node;
    }
    else
    {
        new_node->next = s->nptr;
        s->nptr = new_node;
    }
    s->size++;
}

void pop(stack *s)
{
    if (s->nptr == NULL)
    {
        return;
    }
    else
    {
        node *tmp = s->nptr;
        s->nptr = tmp->next;
        free(tmp);
        s->size--;
    }
}
char *top(stack *s)
{
    if (s->nptr != NULL)
    {
        return s->nptr->element;
    }
    return NULL;
}

void destroystack(stack *s)
{
    if (s == NULL)
    {
        return;
    }
    node *tmp = s->nptr;
    node *prev;
    while (tmp != NULL)
    {
        prev = tmp;
        tmp = tmp->next;
        free(prev);
    }
    free(s);
}
void print_stack(stack *s)
{
    if (s == NULL || s->nptr == NULL)
    {
        return;
    }
    printf("\n");
    node *tmp = s->nptr;
    node *prev = tmp;
    while (tmp != NULL)
    {
        prev = tmp;
        fprintf(stderr, "%s", tmp->element);
        tmp = tmp->next;
        free(prev);
        s->size--;
    }
    s->nptr = NULL;
}