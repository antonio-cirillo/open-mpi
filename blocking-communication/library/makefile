program : mycollective.o main.o
	mpicc mycollective.o main.o -o mpi

main.o :
	mpicc -c main.c

mycollective.o :
	mpicc -c mycollective.c

clean : 
	rm -f mycollective.o main.o mpi