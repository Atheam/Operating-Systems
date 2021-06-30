CC = gcc -Wall


clean:
	rm -f  *.o *.a main

run:
	make clean
	$(CC) main.c -o main -lpthread
	echo "-----BLOCK------" > times.txt
	echo "1 thread" >> times.txt
	./main 1 block balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	echo "2 threads" >> times.txt
	./main 2 block balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	echo "4 threads" >> times.txt
	./main 4 block balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	echo "8 threads" >> times.txt
	./main 8 block balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	echo "16 threads" >> times.txt
	./main 16 block balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	
	echo "-----NUMBERS------" >> times.txt
	echo "1 thread" >> times.txt
	./main 1 numbers balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	echo "2 threads" >> times.txt
	./main 2 numbers balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	echo "4 threads" >> times.txt
	./main 4 numbers balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	echo "8 threads" >> times.txt
	./main 8 numbers balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	echo "16 threads" >> times.txt
	./main 16 numbers balloons.pgm out_file.txt >> times.txt
	echo "" >> times.txt
	
	
	
	make clean


	
	

