CC=colorgcc
all:
	${CC} -g -Wall -Werror -pedantic -std=c99 -o test0 -lm -lGL -lglut src/recalc0.c src/main.c
	${CC} -g -Wall -Werror -pedantic -std=c99 -o test1 -lm -lGL -lglut src/recalc1.c src/main.c 
