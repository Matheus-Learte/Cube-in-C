all:
	gcc cube.c -Wall -std=gnu99 -o cube -lm

clean:
	rm cube

run:
	./cube