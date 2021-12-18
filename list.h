#ifndef LIST_H
#define LIST_H

#include <stddef.h> /* size_t */

#include "node.h" /* struct Node */

struct List {
    struct Node *head;
    size_t length;
};

struct List *init_list(void);
int append(struct List *, struct Node *);
struct Node *remove_head(struct List *);
void free_list(struct List *);

#endif /* LIST_H */