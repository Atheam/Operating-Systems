CC = gcc -Wall 

clean:
	rm -f *.o
	rm -f *.a
	rm -f zad3_a
	rm -f zad3_b
lib:
	$(CC) zad3_a.c -o zad3_a -lm
sys:
	$(CC) zad3_b.c -o zad3_b -lm
run-lib:
	make clean
	make lib
	./zad3_a
	make clean
run-sys:
	make clean
	make sys
	./zad3_b 
	make clean

timeit:
	make clean
	make lib
	make sys
	./zad3_a timeit | tail -5 > pomiar_zad_3.txt
	echo "\n" >> pomiar_zad_3.txt
	./zad3_b timeit | tail -5 >> pomiar_zad_3.txt
	make clean

