#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

struct Token *init_token(char *tok, enum Type t) {
	struct Token *token;
	size_t len;

	if ((token = malloc(sizeof(struct Token))) == NULL) {
		fprintf(stderr, "error allocating memory [token]\n");
		exit(EXIT_FAILURE);
	}

	if ((len = strlen(tok)) > 0xff) {
		fprintf(stderr, "token value too long\n");
		exit(EXIT_FAILURE);
	}

	if ((token->token = malloc(len + 1)) == NULL) {
		fprintf(stderr, "error allocating memory [token->token]\n");
		exit(EXIT_FAILURE);
	}

	strncpy(token->token, tok, len);
	token->token[len] = 0;
	token->type = t;

	return token;
}

void free_token(struct Token *t) {
	if (!t) {
		fprintf(stderr, "invalid pointer state [token]\n");
		exit(EXIT_FAILURE);
	}

	if (!t->token) {
		fprintf(stderr, "invalid pointer state [token->token]\n");
		exit(EXIT_FAILURE);
	}

	free(t->token);
	free(t);
}

void invalid_token(char c) {
	fprintf(stderr, "invalide token [%c]\n", c);
	exit(EXIT_FAILURE);
}
