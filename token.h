#ifndef TOKEN_H
#define TOKEN_H

enum Type {
	Operator = 0,
	Identifier,
	Punct,
	Keyword,
	Constant
};

struct Token {
	char *token;
	enum Type type;
};

struct Token *init_token(char *, enum Type);
void free_token(struct Token *);

void invalid_token(char);

#endif /* TOKEN_H */
