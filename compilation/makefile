output: main.o libmerge.a
	gcc main.o libmerge.a -o output

main.o: main.c
	gcc -c main.c

libmerge.a: merge.o merge.h
	ar rsc libmerge.a merge.o

merge.o:
	gcc -c merge.c
	
clean:
	rm -f *.o *.a output
