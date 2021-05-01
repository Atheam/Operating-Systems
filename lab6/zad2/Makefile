CC = gcc -Wall


clean:
	rm -f  *.o *.a client server

compile:
	make clean
	$(CC) server.c -o server -lrt
	$(CC) -pthread client.c -o client -lrt
	

