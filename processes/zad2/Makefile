CC = gcc -Wall


static:
	$(CC) -c mergelib.c -o mergelib.o
	ar rcs mergelib.a mergelib.o
	
clean:
	rm -f  *.o *.a zad2 test_file*
	
	
run: clean static
	$(CC) -c zad2.c -o zad2.o
	$(CC) -static zad2.o -L. mergelib.a -o zad2
	./zad2 3 a.txt b.txt
	make clean
	
	
timeit: clean static 
	$(CC) -c zad2.c -o zad2.o
	$(CC) -static zad2.o -L. mergelib.a -o zad2
	./zad2 timeit > raport.txt
	make clean
	


