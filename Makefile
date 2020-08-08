CC=g++
LIBS=src/matrix.cpp
FLAGS= -Wall -std=c++11 -g -fopenmp
all: seq omp mpi

seq:
		$(CC)  $(CFLAGS) -o bin/seq $(LIBS) src/seq.cpp

omp:
		$(CC) $(CFLAGS) -fopenmp -o bin/omp $(LIBS) src/omp.cpp

mpi:
		mpic++ $(CFLAGS) -o bin/mpi $(LIBS) src/mpi.cpp
