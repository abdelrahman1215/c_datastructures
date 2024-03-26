test:
	gcc -Wall -g3 -o test.exe test.c -Lbin -lhashmap -llinked_list -ldynamic_array -lAVL_tree

map_test:
	make hashmap
	gcc -Wall -g3 -o HashMapTest.exe tests/HashMapTest.c -Lbin -lhashmap

list_test:
	make linked_list
	gcc -Wall -g3 -o LinkedListTest.exe tests/LinkedListTest.c -Lbin -llinked_list

AVL_test:
	make AVL_tree
	gcc -Wall -g3 -o AVL_treeTest.exe tests/AVL_treeTest.c -Lbin -lAVL_tree

dynamic_array_test:
	make dynamic_array
	gcc -Wall -g3 -o DynamicArrayTest.exe tests/DynamicArrayTest.c -Lbin -ldynamic_array

data_tests:
	make map_test
	make list_test
	make dynamic_array_test
	make AVL_test

hashmap:
	gcc -c src/hashmap.c -o bin/hashmap.o
	ar rcs bin/libhashmap.a bin/hashmap.o
	del bin\hashmap.o
	
linked_list:
	gcc -c src/linked_list.c -o bin/linked_list.o
	ar rcs bin/liblinked_list.a bin/linked_list.o
	del bin\linked_list.o

dynamic_array:
	gcc -c src/dynamic_array.c -o bin/dynamic_array.o
	ar rcs bin/libdynamic_array.a bin/dynamic_array.o
	del bin\dynamic_array.o

AVL_tree:
	gcc -c src/AVL_tree.c -o bin/AVL_tree.o
	ar rcs bin/libAVL_tree.a bin/AVL_tree.o
	del bin\AVL_tree.o

libs:
	make hashmap
	make linked_list
	make dynamic_array
	make AVL_tree