CC = gcc -Wall


clean:
	rm -f  *.o *.a cook delivery main

run:
	make clean
	$(CC) cook.c utils.c -o cook
	$(CC) delivery.c utils.c -o delivery
	$(CC) main.c utils.c -o main
	./main 5 5
	make clean


	
	

