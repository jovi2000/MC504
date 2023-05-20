CFLAGS = -std=c99 -Wall -g -pthread
LDFLAGS = -lm

.PHONY: all clean

all: restaurante

restaurante: restaurante.c
	gcc $(CFLAGS) -c restaurante.c $(LDFLAGS)
	gcc $(CFLAGS) -o restaurante.out restaurante.o $(LDFLAGS)

clean:
	rm -f restaurante
	rm -f *.o
