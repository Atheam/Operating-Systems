CC = gcc -Wall 

clean:
	rm -f  *.o *.a cook delivery main 

run:
	make clean
	$(CC) cook.c utils.c -o cook -lrt -lpthread
	$(CC) delivery.c utils.c -o delivery -lrt -lpthread
	$(CC) main.c utils.c -o main -lrt -lpthread
	./main 5 5
	make clean


	
	

