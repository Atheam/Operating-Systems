CC = gcc -Wall 

clean:
	rm -f *.o
	rm -f *.a
	rm -f zad5_a
	rm -f zad5_b
lib:
	$(CC) zad5_a.c -o zad5_a
sys:
	$(CC) zad5_b.c -o zad5_b
run-lib:
	make clean
	make lib
	./zad5_a a.txt b.txt
	make clean
run-sys:
	make clean
	make sys
	./zad5_b a.txt b.txt
	make clean

timeit:
	make clean
	make lib
	make sys
	./zad5_a testA.txt testB.txt timeit | tail -5 > pomiar_zad_5.txt
	echo "\n" >> pomiar_zad_5.txt
	./zad5_b testA.txt testB.txt timeit | tail -5 >> pomiar_zad_5.txt
	make clean

