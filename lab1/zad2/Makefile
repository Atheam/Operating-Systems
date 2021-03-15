CC = gcc -Wall


static:
	$(CC) -c mergelib.c -o mergelib.o
	ar rcs mergelib.a mergelib.o
clean:
	rm -f  *.o *.a main
	
run: clean static
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. mergelib.a -o main
	./main create_table 2 merge_files a.txt:b.txt c.txt:d.txt remove_block 1 remove_row 0 1
	make clean
timeit: clean static
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. mergelib.a -o main
	./main timeit > raport2.txt
	make clean

