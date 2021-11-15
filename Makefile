CC = g++
CFLAGS = -Wall

all: main

main: main.o Ray.o primitive.o bitmap.o
	$(CC) $(CFLAGS) -o $@ $^ -lm -fopenmp `sdl2-config --cflags --libs`
	
main.o: main.c Ray.h primitive.h
	$(CC) $(CFLAGS) -c $< -lm -fopenmp `sdl2-config --cflags --libs`

Ray.o: Ray.c Ray.h primitive.h
	$(CC) $(CFLAGS) -c $< -lm -fopenmp

primitive.o: primitive.c primitive.h
	$(CC) $(CFLAGS) -c $< -lm

bitmap.o: bitmap.c bitmap.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o
	rm -f main
	rm -rf Output