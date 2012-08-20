CC=gcc -g -Wall -Wextra -std=c99
all:
	${CC} -o test0 -lm -lGL -lglut src/recalc0.c src/main.c
	${CC} -o test1 -lm -lGL -lglut src/recalc1.c src/main.c
