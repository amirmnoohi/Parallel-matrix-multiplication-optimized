CC=g++
LIBS=src/matrix.cpp
all: seq omp mpi

seq:
		$(CC)  -std=c++11 -o bin/seq $(LIBS) src/seq.cpp

omp:
		$(CC) -std=c++11 -fopenmp -o bin/omp $(LIBS) src/omp.cpp

mpi:
		mpic++ -std=c++11 -o bin/mpi $(LIBS) src/mpi.cpp
