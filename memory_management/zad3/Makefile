CC = gcc -Wall


static:
	$(CC) -c mergelib.c -o mergelib.o
	ar rcs mergelib.a mergelib.o
	
shared:
	$(CC) -fPIC -shared mergelib.c -o libmerge.so

clean:
	rm -f  *.o *.a main main_dynamic *.so *.out
	
static-run: clean static
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. mergelib.a -o main
	./main create_table 2 merge_files a.txt:b.txt c.txt:d.txt remove_block 1 remove_row 0 1
	make clean
	
shared-run: clean shared
	$(CC) main.c -L. -Wl,-rpath=. -lmerge -o main
	./main create_table 2 merge_files a.txt:b.txt c.txt:d.txt remove_block 1 remove_row 0 1
	make clean
dynamic-run: clean shared
	$(CC) main_dynamic.c -ldl -o main_dynamic
	./main_dynamic create_table 2 merge_files a.txt:b.txt c.txt:d.txt remove_block 1 remove_row 0 1
	make clean
	
	
timeit-3a:
	echo "\nSTATIC LIBRARY\n" > raport3a.txt
	make clean
	make static
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. mergelib.a -o main
	./main timeit >> raport3a.txt
	
	
	echo "\nSHARED LIBRARY\n" >> raport3a.txt
	make clean
	make shared
	$(CC) main.c -L. -Wl,-rpath=. -lmerge -o main
	./main timeit >> raport3a.txt
	
	echo "\n DYNAMIC LIBRARY\n" >> raport3a.txt
	make clean
	make shared
	$(CC) main_dynamic.c -ldl -o main_dynamic
	./main_dynamic timeit >> raport3a.txt
	make clean	

timeit-3b:
	echo "\nOPTIMIZE -O1\n" > raport3b.txt
	echo "\nSTATIC LIBRARY\n" >> raport3b.txt
	make clean
	make static
	$(CC) -O1 -c main.c -o main.o
	$(CC) -O1 -static main.o -L. mergelib.a -o main
	./main timeit >> raport3b.txt
	
	
	echo "\nSHARED LIBRARY\n" >> raport3b.txt
	make clean
	make shared
	$(CC) -O1 main.c -L. -Wl,-rpath=. -lmerge -o main
	./main timeit >> raport3b.txt
	
	echo "\nDYNAMIC LIBRARY\n" >> raport3b.txt
	make clean
	make shared
	$(CC) -O1 main_dynamic.c -ldl -o main_dynamic
	./main_dynamic timeit >> raport3b.txt
	make clean	
	
	echo "\nOPTIMIZE -O2\n" >> raport3b.txt
	echo "\nSTATIC LIBRARY\n" >> raport3b.txt
	make clean
	make static
	$(CC) -O2 -c main.c -o main.o
	$(CC) -O2 -static main.o -L. mergelib.a -o main
	./main timeit >> raport3b.txt
	
	
	echo "\nSHARED LIBRARY\n" >> raport3b.txt
	make clean
	make shared
	$(CC) -O2 main.c -L. -Wl,-rpath=. -lmerge -o main
	./main timeit >> raport3b.txt
	
	echo "\nDYNAMIC LIBRARY\n" >> raport3b.txt
	make clean
	make shared
	$(CC) -O2 main_dynamic.c -ldl -o main_dynamic
	./main_dynamic timeit >> raport3b.txt
	make clean
	
	echo "\nOPTIMIZE -O3\n" >> raport3b.txt
	echo "\nSTATIC LIBRARY\n" >> raport3b.txt
	make clean
	make static
	$(CC) -O3 -c main.c -o main.o
	$(CC) -O3 -static main.o -L. mergelib.a -o main
	./main timeit >> raport3b.txt
	
	
	echo "\nSHARED LIBRARY\n" >> raport3b.txt
	make clean
	make shared
	$(CC) -O3 main.c -L. -Wl,-rpath=. -lmerge -o main
	./main timeit >> raport3a.txt
	
	echo "\nDYNAMIC LIBRARY\n" >> raport3b.txt
	make clean
	make shared
	$(CC) -O3 main_dynamic.c -ldl -o main_dynamic
	./main_dynamic timeit >> raport3b.txt
	make clean
	
	
	
	
	
	
	
	
	
	
	

