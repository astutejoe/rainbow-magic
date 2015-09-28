
SRC = main.c pbkdf2.c

all:
	gcc -fopenmp -std=c99 $(SRC) -O3 -o rainbow-magic -lcrypto

clean:
	rm -f rainbow-magic
