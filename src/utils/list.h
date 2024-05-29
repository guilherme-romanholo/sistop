#ifndef SISTOP_LIST_H
#define SISTOP_LIST_H

/// List Node struct
typedef struct node {
    void *content;
    struct node *next;
} Node;

/// List struct
typedef struct {
    Node *head;
    Node *tail;
    int size;
} List;

List* List__create();
Node* Node__create(void *);
void List__destroy(List *, void(*)(void *));

void List__append(List *, void *);
void *List__remove_head(List *);
void List__remove_node(List *, void *, int (*)(void *, void*));

#endif //SISTOP_LIST_H
