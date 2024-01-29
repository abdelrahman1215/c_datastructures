test:
	gcc -Wall -g3 -o test.exe test.c -Lbin -lhashmaps -llinked_lists

hashmap:
	gcc -c libsrc/hashmap.c -o bin/hashmap.o
	ar rcs bin/libhashmap.a bin/hashmap.o
	
linked_list:
	gcc -c libsrc/linked_list.c -o bin/linked_list.o
	ar rcs bin/liblinked_list.a bin/linked_list.o

libs:
	make hashmap
	make linked_list