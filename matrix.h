#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <ctime>
#include "mpi.h"
#pragma warning(disable:4996)
using namespace std;

// Support multiple data type
typedef short int T;
typedef chrono::high_resolution_clock C;
typedef chrono::nanoseconds Time;

// Dimension of matrices
#define DIM 4096

// Maximum and Minimum value of random matrices
#define MAX_VAL 100
#define MIN_VAL 1

// Verify Output Calculated Matrix
#define VERIFY false

// Time unit for calculating Performance
#define Time_Unit " nanoseconds"

// Multi Threaded Variables
int workers;
int thread_id;

// Function Decleration
bool VerifyMultiplication(T**, T**, T**);
void prints(string, const char*, int);
string now();


class Matrix {
public:
	enum {

		ALL_MATRIX = 0x1,// set data from input matrix
		ALL_ZERO = 0x2,  // set all element of matrix to zero
		ALL_RANDOM = 0x3 // set all element of matrix randomly
	};

	T** _matrix;
	T* _flat;
	bool _is_row_major;


	Matrix(T**, short int, bool);
	~Matrix();
	void clear();
	void MatrixShow();
	void FlatShow();
	int_fast64_t SimpleMultiply(const Matrix&, Matrix&);
	int_fast64_t FlatMultiply(const Matrix&, Matrix&);
};
