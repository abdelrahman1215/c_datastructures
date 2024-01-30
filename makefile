test:
	gcc -Wall -g3 -o test.exe test.c -Lbin -lhashmap -llinked_list -lc_vector

hashmap:
	gcc -c libsrc/hashmap.c -o bin/hashmap.o
	ar rcs bin/libhashmap.a bin/hashmap.o
	del bin\hashmap.o
	
linked_list:
	gcc -c libsrc/linked_list.c -o bin/linked_list.o
	ar rcs bin/liblinked_list.a bin/linked_list.o
	del bin\linked_list.o

c_vector:
	gcc -c libsrc/c_vector.c -o bin/c_vector.o
	ar rcs bin/libc_vector.a bin/c_vector.o
	del bin\c_vector.o

libs:
	make hashmap
	make linked_list
	make c_vector