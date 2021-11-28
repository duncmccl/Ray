CC = g++
CFLAGS = -Wall

all: main

main: main.o Ray.o primitive.o bitmap.o
	$(CC) $(CFLAGS) -o $@ $^ -lm -fopenmp
	
main.o: main.c Ray.h primitive.h tiny_obj_loader.h
	$(CC) $(CFLAGS) -c $< -lm -fopenmp

Ray.o: Ray.c Ray.h primitive.h
	$(CC) $(CFLAGS) -c $< -lm -fopenmp

primitive.o: primitive.c primitive.h
	$(CC) $(CFLAGS) -c $< -lm -fopenmp

bitmap.o: bitmap.c bitmap.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o
	rm -f main
	rm -rf Output