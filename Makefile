built:
	gcc -o avion avion.c
run:
	./avion date.in rezultate.out
clean:
	rm -f *.o
	rm -f avion
