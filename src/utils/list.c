#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/// Create a linked list
/// \return Return the created linked list
List* List__create() {
    List *list = malloc(sizeof(List));

    if (list == NULL) {
        printf("Not memory to allocate list.");
        exit(1);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
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
    } else {
        list->tail->next = node;
        node->prev = list->tail;
    }

    list->tail = node;
    list->size++;
}

/// Creates a new node with content
/// \param content Node content
/// \return Created node
Node* Node__create(void *content) {
    Node *node = malloc(sizeof(Node));

    if (node == NULL) {
        printf("Not memory to allocate node.");
        return NULL;
    }

    node->content = content;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

/// Remove the first node from a list
/// \param list List
/// \return Node content
void *List__remove_head(List *list) {
    Node *aux = list->head;
    void *content = list->head->content;

    if (list->head) {

        if (list->head->next)
            list->head->next->prev = NULL;
        else
            list->tail = NULL;

        list->head = list->head->next;

        aux->next = NULL;
        aux->prev = NULL;
        list->size--;
    }

    free(aux);

    return content;
}

/// Remove node from list
/// \param list List
/// \param node Node to be removed
void List__remove_node(List *list, Node *node) {
    if(node != NULL)
        return;

    if(node->prev != NULL)
        node->prev->next = node->next;
    else if(list->head == node){
        list->head = node->next;
        list->head->prev = NULL;
    }

    if (node->next != NULL)
        node->next->prev = node->prev;
    else if (list->tail == node){
        list->tail = node->prev;
        list->tail->next = NULL;
    }

    node->next = NULL;
    node->prev = NULL;
    free(node);
    list->size--;
}

/// Remove the tail node from a list
/// \param list List
/// \return Tail Node
Node *List__remove_tail(List *list) {
    Node *aux = list->tail;

    if (list->tail) {
        if (list->tail->prev)
            list->tail->prev->next = NULL;
        else
            list->head = NULL;

        list->tail = list->tail->prev;

        aux->next = NULL;
        aux->prev = NULL;
        list->size--;
    }

    return aux;
}

/// Free the list
/// \param list List
void List__destroy(List *list){
    Node *aux = list->head;

    while (aux != NULL) {
        Node *next = aux->next;
        free(aux);
        aux = next;
    }

    free(list);
}
