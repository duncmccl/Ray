CC = g++
CFLAGS = -Wall

all: main

main: main.o Ray.o primative.o
	$(CC) $(CFLAGS) -o $@ $^ -lm -fopenmp `sdl2-config --cflags --libs`
	
main.o: main.c Ray.h primative.h
	$(CC) $(CFLAGS) -c $< -lm -fopenmp `sdl2-config --cflags --libs`

Ray.o: Ray.c Ray.h primative.h
	$(CC) $(CFLAGS) -c $< -lm -fopenmp

primative.o: primative.c primative.h
	$(CC) $(CFLAGS) -c $< -lm

clean:
	rm -rf *.o
	rm main