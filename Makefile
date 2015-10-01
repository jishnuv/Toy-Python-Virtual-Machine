toypython: vm.c run.o ALOperations.o stack.o
	gcc -Wall vm.c run.o ALOperations.o stack.o -o toypython
run.o: run.c
	gcc -Wall -c run.c
ALOperations.o: ALOperations.c
	gcc -Wall -c ALOperations.c
stack.o: stack.c
	gcc -Wall -c stack.c
clean:
	rm *.o
	rm toypython
