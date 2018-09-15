LIB=-lm

main: main.c aes.c
	gcc -o aes main.c aes.c $(LIB)
