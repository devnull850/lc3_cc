#include <stddef.h>
#include <string.h>

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

int is_keyword(char *token) {
    for (size_t i = 0; keywords[i]; ++i) {
        if (!strncmp(token, keywords[i], strlen(keywords[i]))) {
            return 1;
        }
    }

    return 0;
}