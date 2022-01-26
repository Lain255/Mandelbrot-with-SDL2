LIBS	:= -lSDL2main -lSDL2
SOURCE	:= mandelbrot.c
FLAGS	:= -O3 -Wall

run:
	gcc $(FLAGS) $(SOURCE) $(LIBS)
	./a.out
	rm a.out
