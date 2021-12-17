all: lc3-cc

lc3-cc: main.c
	gcc -Wall -Werror -o lc3-cc main.c

.PHONY:
clean:
	rm lc3-cc
