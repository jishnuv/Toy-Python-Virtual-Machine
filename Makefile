toypython: vm.c run.o ALOperations.o stack.o
	gcc vm.c run.o ALOperations.o stack.o -o toypython
run.o: run.c
	gcc -c run.c
ALOperations.o: ALOperations.c
	gcc -c ALOperations.c
stack.o: stack.c
	gcc -c stack.c
clean:
	rm *.o
	rm toypython
