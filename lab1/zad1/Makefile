CC = gcc -Wall -std=c11 


static:
	$(CC) -c mergelib.c -o mergelib.o
	ar rcs mergelib.a mergelib.o
shared:
	$(CC) -fPIC -shared mergelib.c -o mergelib.so
	

