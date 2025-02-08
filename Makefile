all: test

test: test.o filesystem.o 
		gcc -o test test.o filesystem.o

test.o: test.c filesystem.h
		gcc -c test.c

filesystem.o: filesystem.c filesystem.h
				gcc -c filesystem.c