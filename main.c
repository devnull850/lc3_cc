#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "node.h"
#include "parse.h"
#include "token.h"

int main(int argc, char *argv[]) {
	FILE *fd;
	char *buf;
	size_t len, i;
	struct Node *n;
	struct List *tokens;
	char tok[0x100];
	enum Type t;

	if (argc < 2) {
		puts("usage: ./lc3-cc <filename>");
		return EXIT_SUCCESS;
	}

	if ((fd = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "error opening file [%s]\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	if ((buf = malloc(len+1)) == NULL) {
		fprintf(stderr, "error allocating memory [file buffer]\n");
		exit(EXIT_FAILURE);
	}

	fread(buf, 1, len, fd);
	buf[len] = 0;

	if (fclose(fd) == EOF) {
		fprintf(stderr, "error closing file [%s]\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	tokens = init_list();

	for (char *s = buf; *s; ++s) {
		if (isdigit(*s)) {
			i = 0;

			tok[i] = *s;
			t = Constant;
			++s;

			if (!(*s) || isalpha(*s)) {
				invalid_token(*s);
			}

			while (isdigit(*s)) {
				tok[++i] = *s;
				++s;
			}

			tok[++i] = 0;

			n = init_node(tok, t);
			append(tokens, n);
		}
		else if (isalpha(*s)) {
			i = 0;

			tok[i] = *s;
			t = Identifier;
			++s;

			if (!(*s)) {
				invalid_token(*s);
			}

			while (isalnum(*s)) {
				tok[++i] = *s;
				++s;
			}

			tok[++i] = 0;
			
			n = init_node(tok, t);
			append(tokens, n);
		}

		switch (*s) {
		case '/':
			++s;
			if (!(*s)) {
				invalid_token(*s);
			}

			if (*s == '/') {
				++s;
				while (*s && *s != '\n') {
					++s;
				}
				break;
			}
			else if (*s == '*') {
				++s;

				while ((*s) && (*s != '*' || *(s+1) != '/')) {
					++s;
				}

				++s;
				break;
			}

			invalid_token(*s);
		case '\n':
		case '\t':
		case '\r':
		case ' ':	
			break;
		case '"':
			++s;
			if (!(*s)) {
				invalid_token(*s);
			}

			i = 0;
			t = Constant;

			while (*s && *s != '"') {
				tok[i++] = *s; 
				++s;
			}

			if (*s == '"') {
				tok[i] = 0;
				n = init_node(tok, t);
				append(tokens, n);
				break;
			}

			invalid_token(*s);
		case '{':
		case '}':
		case '[':
		case ']':
		case '(':
		case ')':
		case ';':
		case ',':
			t = Punct;
			tok[0] = *s;
			tok[1] = 0;
			n = init_node(tok, t);
			append(tokens, n);
			break;
		case '&':
		case '+':
		case '-':
		case '~':
		case '>':
		case '<':
		case '!':
		case '=':
			tok[0] = *s;
			++s;

			if (!(*s)) {
				invalid_token(*s);
			}

			t = Operator;

			if (*s == '=') {
				tok[1] = *s;
				tok[2] = 0;
			}
			else {
				--s;
				tok[1] = 0;
			}

			n = init_node(tok, t);
			append(tokens, n);
			break;
		default:
			invalid_token(*s);
		};
	}

	if (!buf) {
		fprintf(stderr, "invalid pointer state [file buffer]\n");
		exit(EXIT_FAILURE);
	}

	free(buf);

	for (n = tokens->head; n; n = n->next) {
		printf("%s %d %d\n", n->token->token, n->token->type, is_keyword(n->token->token));
	}

	free_list(tokens);

	return EXIT_SUCCESS;
}
