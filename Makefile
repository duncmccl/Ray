CC = g++
CFLAGS = -Wall

all: main

main: main.o Ray.o
	$(CC) $(CFLAGS) -o $@ $^ -lm
	
main.o: main.c Ray.h primative.h
	$(CC) $(CFLAGS) -c $< -lm

Ray.o: Ray.c Ray.h primative.h
	$(CC) $(CFLAGS) -c $< -lm

clean:
	rm -rf *.o
	rm main