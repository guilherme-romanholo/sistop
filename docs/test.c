#include "../src/utils/list.h"
#include <stdlib.h>
#include <stdio.h>

int compare_int(void *x, void *y) {
    return *((int *)x) - *((int *)y);
}

int main() {
    int *i = malloc(sizeof(int));
    int *j = malloc(sizeof(int));

    *i = 2;
    *j = 3;

    List *list = List__create();

    List__append(list, (void *) i);
    List__append(list, (void *) j);

    Node *aux = list->head;

    printf("List:\n");
    while (aux != NULL) {
        printf("%d\n", *((int *)aux->content));
        aux = aux->next;
    }

    List__remove_node(list, (void *)j, compare_int);

    aux = list->head;

    printf("Remove node:\n");
    while (aux != NULL) {
        printf("%d\n", *((int *)aux->content));
        aux = aux->next;
    }

    int *z = (int *) List__remove_head(list);
    printf("Removed head: %d\n", *z);

    return 0;
}