CC = gcc


map_test: hashmap
	$(CC) -Wall -g3 -o HashMapTest.exe tests/HashMapTest.c -Lbin -lhashmap

list_test: linked_list
	$(CC) -Wall -g3 -o LinkedListTest.exe tests/LinkedListTest.c -Lbin -llinked_list

AVL_test: AVL_tree
	$(CC) -Wall -g3 -o AVL_treeTest.exe tests/AVL_treeTest.c -Lbin -lAVL_tree

dynamic_array_test: dynamic_array
	$(CC) -Wall -g3 -o DynamicArrayTest.exe tests/DynamicArrayTest.c -Lbin -ldynamic_array

data_tests:
	$(MAKE) map_test
	$(MAKE) list_test
	$(MAKE) dynamic_array_test
	$(MAKE) AVL_test

hashmap: bin/
	$(CC) -c src/hashmap.c -o bin/hashmap.o
	ar rcs bin/libhashmap.a bin/hashmap.o
	
linked_list:bin/
	$(CC) -c src/linked_list.c -o bin/linked_list.o
	ar rcs bin/liblinked_list.a bin/linked_list.o

dynamic_array:bin/
	$(CC) -c src/dynamic_array.c -o bin/dynamic_array.o
	ar rcs bin/libdynamic_array.a bin/dynamic_array.o

AVL_tree:bin/
	$(CC) -c src/AVL_tree.c -o bin/AVL_tree.o
	ar rcs bin/libAVL_tree.a bin/AVL_tree.o

libs:
	$(MAKE) hashmap
	$(MAKE) linked_list
	$(MAKE) dynamic_array
	$(MAKE) AVL_tree
	
bin/:
	$(shell mkdir bin)