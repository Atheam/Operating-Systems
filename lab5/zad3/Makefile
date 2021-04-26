CC = gcc -Wall


clean:
	rm -f  *.o *.a producent fifo consumer


run-producent:
	make clean
	mkfifo fifo
	$(CC) producent.c -o producent
	./producent fifo 3 plik.txt 3
	
run-consumer:
	$(CC) consumer.c -o consumer
	./consumer fifo out.txt 3
	make clean
	
