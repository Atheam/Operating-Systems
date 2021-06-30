CC = gcc -Wall 

clean:
	rm -f *.o
	rm -f *.a
	rm -f zad2_a
	rm -f zad2_b
lib:
	$(CC) zad2_a.c -o zad2_a
sys:
	$(CC) zad2_b.c -o zad2_b
run-lib:
	make clean
	make lib
	./zad2_a c a.txt
	make clean
run-sys:
	make clean
	make sys
	./zad2_b c a.txt 
	make clean

timeit:
	make clean
	make lib
	make sys
	./zad2_a B testA.txt timeit | tail -5 > pomiar_zad_2.txt
	echo "\n" >> pomiar_zad_2.txt
	./zad2_b B testA.txt timeit | tail -5 >> pomiar_zad_2.txt
	make clean

