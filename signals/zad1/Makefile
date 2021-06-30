CC = gcc -Wall



clean:
	rm -f  *.o *.a exec main_fork main_exec
	

run:
	make clean
	$(CC) main_exec.c -o main_exec
	$(CC) main_fork.c -o main_fork
	$(CC) exec.c -o exec
	echo "FORK TEST" > raport2.txt
	echo "Ignore test" >> raport2.txt
	./main_fork ignore >> raport2.txt
	echo "Handler test" >> raport2.txt
	./main_fork handler >> raport2.txt
	echo "Mask test" >> raport2.txt
	./main_fork mask >> raport2.txt
	echo "Pending test" >> raport2.txt
	./main_fork pending >> raport2.txt
	echo "EXEC TEST" >> raport2.txt
	echo "Ignore test" >> raport2.txt
	./main_exec ignore >> raport2.txt
	echo "Mask test" >> raport2.txt
	./main_exec mask >> raport2.txt
	echo "Pending test" >> raport2.txt
	./main_exec pending >> raport2.txt
	make clean


