//#include <stdint.h>	/* uint16_t, int16_t */
#include <stdio.h>
#include <stdlib.h>

enum Type {
	Operator = 0,
	Identifier,
	Punct,
	Keyword,
	Constant
};

struct Token {
	char token[0x100];
	enum Type type;
};

int is_digit(char);
int is_alpha(char);

int main(int argc, char *argv[]) {
	FILE *fd;
	char *buf;
	size_t len, index, i;
	struct Token *tokens;

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
		fprintf(stderr, "memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	fread(buf, 1, len, fd);
	buf[len] = EOF;

	if (fclose(fd) == EOF) {
		fprintf(stderr, "error closing file [%s]\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ((tokens = malloc(sizeof(struct Token) * len)) == NULL) {
		fprintf(stderr, "error allocating token array\n");
		exit(EXIT_FAILURE);
	}

	index = 0;

	for (char *s = buf; *s; ++s) {
		if (*s == EOF) {
			break;
		}

		if (is_digit(*s)) {
			i = 0;

			tokens[index].token[i] = *s;
			tokens[index].type = Constant;
			++s;

			if (!(*s) || is_alpha(*s)) {
				goto invalid_token;
			}

			while (is_digit(*s)) {
				tokens[index].token[++i] = *s;
				++s;
			}

			tokens[index].token[++i] = 0;
			++index;
		}
		else if (is_alpha(*s)) {
			i = 0;

			tokens[index].token[i] = *s;
			tokens[index].type = Identifier;
			++s;

			if (!(*s)) {
				goto invalid_token;
			}

			while (is_digit(*s) || is_alpha(*s)) {
				tokens[index].token[++i] = *s;
				++s;
			}

			tokens[index].token[++i] = 0;
			++index;
		}

		switch (*s) {
		case '/':
			++s;
			if (!(*s)) {
				goto invalid_token;
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

			goto invalid_token;
		case '\n':
		case '\t':
		case '\r':
		case ' ':	
			break;
		case '"':
			++s;
			if (!(*s)) {
				goto invalid_token;
			}

			i = 0;
			tokens[index].type = Constant;

			while (*s && *s != '"') {
				tokens[index].token[i++] = *s; 
				++s;
			}

			if (*s == '"') {
				tokens[index].token[i] = 0;
				++index;
				break;
			}

			goto invalid_token;
		case '{':
		case '}':
		case '[':
		case ']':
		case '(':
		case ')':
		case ';':
		case ',':
			tokens[index].type = Punct;
			tokens[index].token[0] = *s;
			tokens[index].token[1] = 0;
			++index;
			break;
		case '&':
		case '+':
		case '-':
		case '~':
		case '>':
			tokens[index].type = Operator;
			tokens[index].token[0] = *s;
			tokens[index].token[1] = 0;
			++index;
			break;
		case '=':
			tokens[index].token[0] = *s;
			++s;

			if (!(*s)) {
				goto invalid_token;
			}

			tokens[index].type = Operator;

			if (*s == '=') {
				tokens[index].token[1] = *s;
				tokens[index].token[2] = 0;
			}
			else {
				--s;
				tokens[index].token[1] = 0;
			}

			++index;
			break;
		case '<':
			tokens[index].token[0] = *s;
			++s;

			if (!(*s)) {
				goto invalid_token;
			}

			tokens[index].type = Operator;

			if (*s == '>') {
				tokens[index].token[1] = *s;
				tokens[index].token[2] = 0;
			}
			else {
				--s;
				tokens[index].token[1] = 0;
			}

			++index;
			break;
		default:
			goto invalid_token;
		};
	}

	if (!buf) {
		goto invalid_pointer;
	}

	free(buf);

	for (i = 0; i < index; ++i) {
		printf("%s\t\t\t\t\t%d\n", tokens[i].token, tokens[i].type);
	}

	if (!tokens) {
		goto invalid_pointer;
	}	

	free(tokens);

	return EXIT_SUCCESS;

invalid_token:
	fprintf(stderr, "invalid token encountered\n");
	exit(EXIT_FAILURE);
invalid_pointer:
	fprintf(stderr, "invalid pointer state\n");
	exit(EXIT_FAILURE);
}

int is_digit(char c) {
	return c >= 0x30 && c < 0x3a;
}

int is_alpha(char c) {
	return (c >= 0x41 && c < 0x5b) ||
		(c >= 0x61 && c < 0x7b);
}
