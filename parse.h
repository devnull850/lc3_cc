#ifndef PARSE_H
#define PARSE_H

#include <stddef.h>

#include "list.h"
#include "token.h"

enum Operator {
	Add = 0,
	Sub,
	Mul,
	Div,
	Rem,
	Andand,
	Oror,
	Not,
	Lt,
	Gt,
	Eq,
	Ne,
	Gte,
	Lte,
	And,
	Or,
	Neg,
	Assign,
	Addr,
	Dref,
	Jmp,
	Lit
};

struct TreeNode {
	enum Type type;
	enum Operator op;
	struct TreeNode *parent;
	struct TreeNode **children;
	size_t num_children;
};

struct Tree {
	struct TreeNode *root;
	size_t length;
};

extern const char *keywords[];

int is_keyword(char *);
struct Tree *parse_tokens(struct List *);
void free_tree(struct Tree *);

#endif /* PARSE_H */
