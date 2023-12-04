test:
	gcc -Wall -g3 -o test.exe test.c -Lbin -lhashmaps -llinked_lists

hashmaps:
	gcc -c libsrc/hashmaps.c -o bin/hashmaps.o
	ar rcs bin/libhashmaps.a bin/hashmaps.o
	
lists:
	gcc -c libsrc/linked_lists.c -o bin/linked_lists.o
	ar rcs bin/liblinked_lists.a bin/linked_lists.o

libs:
	make hashmaps
	make lists