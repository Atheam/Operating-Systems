CC = gcc -Wall 

clean:
	rm -f *.o
	rm -f *.a
	rm -f zad4_a
	rm -f zad4_b
lib:
	$(CC) zad4_a.c -o zad4_a
sys:
	$(CC) zad4_b.c -o zad4_b
run-lib:
	make clean
	make lib
	./zad4_a a.txt b.txt aaaaa bbbbb
	make clean
run-sys:
	make clean
	make sys
	./zad4_b a.txt b.txt aaaaa bbbbb
	make clean

timeit:
	make clean
	make lib
	make sys
	./zad4_a testA.txt testB.txt aaaaa bbbbb timeit | tail -5 > pomiar_zad_4.txt
	echo "\n" >> pomiar_zad_4.txt
	./zad4_b testA.txt testB.txt aaaaa bbbbb timeit | tail -5 >> pomiar_zad_4.txt
	make clean

