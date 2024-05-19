#ifndef SISTOP_LIST_H
#define SISTOP_LIST_H

/// List Node struct
typedef struct node {
    void *content;
    struct node *next;
    struct node *prev;
} Node;

/// List struct
typedef struct list {
    Node *head;
    Node *tail;
    int size;
} List;

List* List__create();
void List__append(List *, void *);
Node* Node__create(void *);
void *List__remove_head(List *list);
void List__remove_node(List *list, Node *node);
Node *List__remove_tail(List *list);
void List__destroy(List *list);

#endif //SISTOP_LIST_H
