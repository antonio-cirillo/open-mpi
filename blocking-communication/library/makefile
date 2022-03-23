int : main.o int-item.o mycollective.o
	mpicc main.o int-item.o mycollective.o -o driver.out

char : main.o char-item.o mycollective.o
	mpicc main.o char-item.o mycollective.o -o driver.out

int-item.o:
	mpicc -c int-item.c

char-item.o:
	mpicc -c char-item.c

main.o :
	mpicc -c main.c

mycollective.o :
	mpicc -c mycollective.c

clean: 
	rm -f main.o int-item.o char-item.o mycollective.o driver.out