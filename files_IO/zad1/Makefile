CC = gcc -Wall 

clean:
	rm -f *.o
	rm -f *.a
	rm -f zad1_a
	rm -f zad1_b
lib:
	$(CC) zad1_a.c -o zad1_a
sys:
	$(CC) zad1_b.c -o zad1_b
run-lib:
	make clean
	make lib
	./zad1_a a.txt b.txt
	make clean
run-sys:
	make clean
	make sys
	./zad1_b a.txt b.txt
	make clean

timeit:
	make clean
	make lib
	make sys
	./zad1_a testA.txt testB.txt timeit | tail -5 > pomiar_zad_1.txt
	echo "\n" >> pomiar_zad_1.txt
	./zad1_b testA.txt testB.txt timeit | tail -5 >> pomiar_zad_1.txt
	make clean

