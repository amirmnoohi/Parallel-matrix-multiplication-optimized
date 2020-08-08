CC=g++
LIBS=src/matrix.cpp

all: seq omp mpi

seq:
		$(CC)  -o bin/seq $(LIBS) src/seq.cpp

omp:
		$(CC) -fopenmp -o bin/omp $(LIBS) src/omp.cpp

mpi:
		mpic++ -o bin/mpi $(LIBS) src/mpi.cpp
