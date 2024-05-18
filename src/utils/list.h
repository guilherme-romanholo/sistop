//
// Created by guilherme on 07/05/24.
//

#ifndef SISTOP_LIST_H
#define SISTOP_LIST_H

/// List Node struct
typedef struct node {
    void *content;
    struct node *next;
} Node;

/// List struct
typedef struct list {
    Node *head;
    Node *tail;
    int size;
} List;

List* List__create();
void List__destroy(List *list);
void List__append(List *, void *);
void *List__remove_first(List *list);
Node* Node__create(void *);

#endif //SISTOP_LIST_H
