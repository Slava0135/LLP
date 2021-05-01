collect: gardener tests

gardener: gardener.o libtree.a
	gcc gardener.o libtree.a -o gardener

tests: tests.o libtree.a
	gcc tests.o libtree.a -o tests

gardener.o: gardener.c
	gcc -c gardener.c
	
tests.o: tests.c
	gcc -c tests.c

libtree.a: tree.o tree.h
	ar rsc libtree.a tree.o

tree.o:
	gcc -c tree.c
	
clean:
	rm -f *.o *.a gardener tests
