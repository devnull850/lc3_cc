#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "parse.h"

const char *keywords[] = {
	"break",
	"char",
	"continue",
	"else",
	"enum",
	"for",
	"if",
	"int",
	"return",
	"signed",
	"struct",
	"unsigned",
	"void",
	"while",
	NULL
};

static struct Tree *init_tree(void) {
    struct Tree *tree;

    if ((tree = malloc(sizeof(struct Tree))) == NULL) {
        fprintf(stderr, "error allocating new struct Tree\n");
        exit(EXIT_FAILURE);
    }

    tree->root = NULL;
    tree->length = 0;

    return tree;
}

// static struct TreeNode *init_tree_node(enum Type type, enum Operator op, struct TreeNode *parent) {
//     struct TreeNode *node;

//     if ((node = malloc(sizeof(struct TreeNode))) == NULL) {
//         fprintf(stderr, "error allocating new struct TreeNode\n");
//         exit(EXIT_FAILURE);
//     }

//     node->type = type;
//     node->op = op;
//     node->parent = parent;
//     node->children = NULL;
//     node->num_children = 0;

//     return node;
// }

// static void free_tree_node(struct TreeNode *n) {
//     if (!n) {
//         fprintf(stderr, "invalid pointer state [struct TreeNode]\n");
//         exit(EXIT_FAILURE);
//     }

//     free(n);
// }

static struct TreeNode *generate_tree_node(struct Node *stack, size_t top, size_t length) {
    for (size_t i = top; i < length; ++i) {
        printf("%d %s\n", stack[i].token->type, stack[i].token->token);
    }

    return NULL;
}

int is_keyword(char *token) {
    for (size_t i = 0; keywords[i]; ++i) {
        if (!strncmp(token, keywords[i], strlen(keywords[i]))) {
            return 1;
        }
    }

    return 0;
}

static void identify_keywords(struct List *tokens) {
    for (struct Node *n = tokens->head; n; n = n->next) {
        if (is_keyword(n->token->token)) {
            n->token->type = Keyword;
        }
    }
}

static size_t find_opening_punct(struct Node *stack, size_t top, size_t length, char tok) {
    for (size_t i = top; i < length; ++i) {
        if (stack[i].token->token[0] == tok) {
            return i+1;
        }
    }

    fprintf(stderr, "parser stack index out of range\n");
    exit(EXIT_FAILURE);
}

struct Tree *parse_tokens(struct List *tokens) {
    struct Tree *tree;
    struct Node stack[tokens->length];
    // struct TreeNode *tnode, *curr;
    size_t i, j;

    tree = init_tree();
    // curr = tree->root;
    i = tokens->length;
    identify_keywords(tokens);

    for (struct Node *n = tokens->head; n; n = n->next) {
        switch (n->token->type) {
            case Operator:
            case Identifier:
            case Keyword:
            case Constant:
                stack[--i] = *n;
                break;
            case Punct:
                switch (n->token->token[0]) {
                    case '{':
                    case '(':
                    case '[':
                        stack[--i] = *n;
                        break;
                    case '}':
                        stack[--i] = *n;
                        j = find_opening_punct(stack, i, tokens->length, '{');
                        generate_tree_node(stack, i, j);
                        i = j;
                        break;
                    case ')':
                        stack[--i] = *n;
                        j = find_opening_punct(stack, i, tokens->length, '(');
                        generate_tree_node(stack, i, j);
                        i = j;
                        break;
                    case ']':
                        stack[--i] = *n;
                        j = find_opening_punct(stack, i, tokens->length, '[');
                        generate_tree_node(stack, i, j);
                        i = j;
                        break;
                    case ';':
                    case ',':
                        stack[--i] = *n;
                        break;
                };
                break;
            default:
                fprintf(stderr, "invalid type [%d]\n", n->token->type);
                exit(EXIT_FAILURE);
        };
	}

    free_list(tokens);

    return tree;
}

void free_tree(struct Tree *tree) {
    if (!tree) {
        fprintf(stderr, "invalid pointer state [struct Tree]\n");
        exit(EXIT_FAILURE);
    }

    free(tree);
}