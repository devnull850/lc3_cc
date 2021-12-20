CFLAGS=-Wall -Werror
OBJ=list.o node.o token.o parse.o

all: lc3-cc

lc3-cc: main.c $(OBJ)
	gcc $(CFLAGS) -o lc3-cc main.c $(OBJ)

list.o: list.c
	gcc $(CFLAGS) -c list.c

node.o: node.c
	gcc $(CFLAGS) -c node.c

token.o: token.c
	gcc $(CFLAGS) -c token.c

parse.o: parse.c
	gcc $(CFLAGS) -c parse.c

.PHONY:
clean:
	rm lc3-cc $(OBJ)
