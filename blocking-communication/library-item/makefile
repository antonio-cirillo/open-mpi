int : broadcasting.o int-item.o mycollective.o
	mpicc broadcasting.o int-item.o mycollective.o -o driver.out

char : broadcasting.o char-item.o mycollective.o
	mpicc broadcasting.o char-item.o mycollective.o -o driver.out

int-item.o:
	mpicc -c int-item.c

char-item.o:
	mpicc -c char-item.c

broadcasting.o :
	mpicc -c broadcasting.c

mycollective.o :
	mpicc -c mycollective.c

clean: 
	rm -f broadcasting.o int-item.o char-item.o mycollective.o driver.out