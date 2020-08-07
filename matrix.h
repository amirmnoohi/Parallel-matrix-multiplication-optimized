#include <iostream>
#include <string>
#include <utility>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <ctime>
#pragma warning(disable:4996)
using namespace std;


typedef chrono::high_resolution_clock T;
typedef chrono::seconds Time;

// Dimension of matrices
#define DIM 8

// Maximum and Minimum value of random matrices
#define MAX_VAL 100
#define MIN_VAL 1

// Verify Output Calculated Matrix
#define VERIFY true

// Time unit for calculating Performance
#define Time_Unit " seconds"

//// Multi Threaded Variables
extern int NUMBER_OF_THREADS;
extern int THREAD_ID;
extern string PARALLEL_TYPE;
extern string METHOD;


// Function Decleration
void help(int = 0);
bool VerifyMultiplication(int**, int**, int**);
void prints(string, const char*, int);
string now();
int** SampleA1();
int** SampleA2();

class Matrix {
public:
	enum {
		ALL_MATRIX = 0x1,// set data from input matrix
		ALL_ZERO = 0x2,  // set all element of matrix to zero
		ALL_RANDOM = 0x3 // set all element of matrix randomly
	};
	int** _matrix;
	int* _flat;
	bool _is_row_major;


	void Init(int**, short int, bool);
	Matrix();
	~Matrix();
	void Clear();
	void FlatShow();
	void MatrixShow();
	void SaveToMatrix();
	friend void FlatRMultiply(int[DIM * DIM], int[DIM * DIM], int[DIM * DIM]);
	friend void FlatCMultiply(int[DIM * DIM], int[DIM * DIM], int[DIM * DIM]);
};
