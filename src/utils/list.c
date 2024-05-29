#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/// Create a linked list
/// \return Return the created linked list
List* List__create() {
    List *list = malloc(sizeof(List));

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

/// Creates a new node with content
/// \param content Node content
/// \return Created node
Node* Node__create(void *content) {
    Node *node = malloc(sizeof(Node));

    node->content = content;
    node->next = NULL;

    return node;
}

/// Append a new node in the list
/// \param list List to append node
/// \param content Node content
void List__append(List *list, void *content) {
    Node *node = Node__create(content);

    if (node == NULL)
        return;

    if (list->head == NULL) {
        list->head = node;
    }

    if (list->tail != NULL){
        list->tail->next = node;
    }

    list->tail = node;
    list->size++;
}

/// Remove the first node from a list
/// \param list List
/// \return Node content
void *List__remove_head(List *list) {
    if (list->head == NULL)
        return NULL;

    Node *removed = list->head;
    void *content = list->head->content;

    list->head = list->head->next;

    if (list->head == NULL)
        list->tail = NULL;

    free(removed);
    list->size--;

    return content;
}

/// Remove node from list
/// \param list List
/// \param node Node to be removed
void List__remove_node(List *list, void *data, int (*compare)(void *, void*), void(*free_func)(void *)) {
    Node *curr = list->head;
    Node *prev = NULL;

    while (curr != NULL && compare(curr->content, data)) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != NULL) {
        if (prev == NULL)
            list->head = curr->next;
        else
            prev->next = curr->next;

        if (curr == list->head)
            list->tail = prev;

        free_func(data);
        free(curr);
        list->size--;
    }
}

/// Free the list
/// \param list List
void List__destroy(List *list, void(*free_func)(void *)){
    Node *curr = list->head;

    while (curr != NULL) {
        Node *next = curr->next;
        free_func(curr->content);
        free(curr);
        curr = next;
    }

    free(list);
}
