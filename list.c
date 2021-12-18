#include <stdio.h>
#include <stdlib.h>

#include "list.h"

static struct Node *get_tail(struct List *list) {
    struct Node *tail, *prev;

    if (!(tail = list->head)) {
        return NULL;
    }

    while (tail) {
        prev = tail;
        tail = tail->next;
    }

    return prev;
}

struct List *init_list(void) {
    struct List *list;

    if ((list = malloc(sizeof(struct List))) == NULL) {
		fprintf(stderr, "error allocating memory [list]\n");
		exit(EXIT_FAILURE);
	}

	list->head = NULL;
	list->length = 0;

    return list;
}

int append(struct List *list, struct Node *n) {
    struct Node *tail;
    
    if ((tail = get_tail(list))) {
        tail->next = n;
    }
    else {
        list->head = n;
    }

    list->length += 1;

    return 1;
}

struct Node *remove_head(struct List *list) {
    struct Node *head;

    if (!(head = list->head)) {
        return NULL;
    }

    list->head = head->next;
    head->next = NULL;
    list->length -= 1;

    return head;
}

void free_list(struct List *list) {
    struct Node *n;

    if (!list) {
        fprintf(stderr, "invalid pointer state [list]\n");
        exit(EXIT_FAILURE);
    }

    while ((n = remove_head(list))) {
        free_node(n);
    }

    free(list);
}