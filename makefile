test:
	gcc -Wall -g3 -o test.exe test.c -Lbin -lhashmaps

binary:
	gcc -c libsrc/hashmaps.c -o bin/hashmaps.o
	ar rcs bin/libhashmaps.a bin/hashmaps.o