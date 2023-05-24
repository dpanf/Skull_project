all: a.out

a.out: Skullc.o lodepng.o
	gcc  $^ -o $@

Skullc.o: Skullc.c lodepng.h
	gcc -c $< -o $@

lodepng.o: lodepng.c lodepng.h
	gcc -c $< -o $@

clean:
	rm -f *.o *.out
