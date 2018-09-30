LIB=-lm

main: main.c aes.c
	gcc -g -Wall -Wextra -o aes main.c aes.c $(LIB)
