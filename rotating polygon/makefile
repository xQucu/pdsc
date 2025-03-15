all: main

main: main.o primlib.o
	gcc -g $^ -o $@  -lSDL2_gfx `sdl2-config --libs` -lm

.c.o: 
	gcc -g -Wall -pedantic `sdl2-config --cflags` -c  $<

primlib.o: primlib.c primlib.h

main.o: main.c primlib.h

clean:
	-rm primlib.o main.o main