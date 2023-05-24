all: a.out

a.out: Skullc.o lodepng.o
	gcc Skullc.o lodepng.o -o a.out

Skullc.o: Skullc.c
	gcc -c Skullc.c

lodepng.o: lodepng.c lodepng.h
	gcc -c lodepng.c lodepng.h

clean:
	rm -f *.o *.out