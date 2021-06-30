CC = gcc -Wall


clean:
	rm -f  *.o *.a client server

compile:
	make clean
	$(CC) server.c -o server
	$(CC) -pthread client.c -o client
	

