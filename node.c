#include <stdio.h>
#include <stdlib.h>

#include "node.h"

struct Node *init_node(char *s, enum Type t) {
    struct Node *n;

    if ((n = malloc(sizeof(struct Node))) == NULL) {
        fprintf(stderr, "error allocating memory [node]\n");
		exit(EXIT_FAILURE);
    }

    n->token = init_token(s, t);
    n->next = NULL;

    return n;
}

void free_node(struct Node *n) {
    if (!n) {
        fprintf(stderr, "invalid pointer state [node]\n");
		exit(EXIT_FAILURE);
    }

    free_token(n->token);
    free(n);
}