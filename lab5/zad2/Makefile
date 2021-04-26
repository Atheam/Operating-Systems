CC = gcc -Wall


clean:
	rm -f  *.o *.a zad2

run-1:
	make clean
	$(CC) zad2.c -o zad2
	./zad2 nadawca
	make clean
run-3:
	make clean
	$(CC) zad2.c -o zad2
	./zad2 $(address) example_title example_content
	make clean 

