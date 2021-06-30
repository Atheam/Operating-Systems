cc = gcc -Wall -ggdb

run: 
	$(cc) client.c -o client -lpthread
	$(cc) server.c -o server -lpthread
	./server 8888 socket
	


