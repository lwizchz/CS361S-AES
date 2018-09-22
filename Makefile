LIB=-lm

main: main.c aes.c
	gcc -g -o aes main.c aes.c $(LIB)
