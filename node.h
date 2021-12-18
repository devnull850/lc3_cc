#ifndef NODE_H
#define NODE_H

#include "token.h"

struct Node {
	struct Token *token;
	struct Node *next;
};

struct Node *init_node(char *, enum Type);
void free_node(struct Node *);

#endif /* NODE_H */
