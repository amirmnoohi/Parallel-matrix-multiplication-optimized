#include "matrix.h"
#include "omp.h"

// Initial Variables
int THREAD_ID;
int NUMBER_OF_THREADS;
string PARALLEL_TYPE = "MPI";
string Method = "C";

// Flat Row Major Multiply
int_fast64_t  FlatRMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Flat Multiplying Started" << endl;
	int IB, JB, KB, temp;
	auto pre = T::now();
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			for (int k = 0; k < DIM; k++)
			{
				final[i * DIM + k] += left[i * DIM + k] * right[j * DIM + k];
			}
		}
	}
	cout << "\t" << now() << " : " << "Flat Multiplying Finished" << endl;
	return  chrono::duration_cast<Time>(T::now() - pre).count();
}

// Flat Column Major Multiply
int_fast64_t  FlatCMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Flat Multiplying Started" << endl;
	int IB, JB, KB, temp;
	auto pre = T::now();
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			for (int k = 0; k < DIM; k++)
			{
				final[i*DIM + k] += left[i * DIM + k] * right[j * DIM + k];
			}
		}
	}
	cout << "\t" << now() << " : " << "Flat Multiplying Finished" << endl;
	return  chrono::duration_cast<Time>(T::now() - pre).count();
}


int main(int argc, char** argv) {

	string output;
	int_fast64_t time;


	if (argc != 3) {
		cout << "ERROR: Please Specify Method: " << endl;
		cout << "e.g: mpicc -np 4 a.out C";
		help(3);
		exit(EXIT_FAILURE);
	}
	if (DIM % NUMBER_OF_THREADS != 0) {
		cout << "ERROR: Matrix can not be calculated with this number of tasks.\n";
		exit(EXIT_FAILURE);
	}
	help(3);

	Matrix A;
	Matrix B;
	Matrix C;


	output = string(" Phase 1 : Matrix Creation ");
	prints(output, "#", 100);
	A.Init(SampleA1(), Matrix::ALL_RANDOM, true);
	if (argv[1] == "B")
		B.Init(SampleA2(), Matrix::ALL_RANDOM, false);
	else
		B.Init(SampleA2(), Matrix::ALL_RANDOM, true);
	C.Init(NULL, Matrix::ALL_ZERO, true);


	// Method B
	if (argv[1] == "B") {

		Method = "B";
		string output = string(" Method ") + Method + string(" - Phase 2 : Matrix Multiplying ");
		prints(output, "#", 100);

		time = FlatRMultiply(A._flat, B._flat, C._flat);
		cout << "\tTime: " << time << endl;

		if (VERIFY) {
			A.SaveToMatrix();
			B.SaveToMatrix();
			C.SaveToMatrix();
			output = string(" Method ") + Method + string(" - Phase 3 : Matrix Verifying ");
			prints(output, "#", 100);
			cout << "\tResult is :" << (VerifyMultiplication(A._matrix, B._matrix, C._matrix) ? " Verified" : " Wrong") << endl;
		}
	}


	// Method E
	if (argv[1] == "E") {
		Method = "E";
		output = string(" Method ") + Method + string(" - Phase 2 : Matrix Multiplying ");
		prints(output, "#", 100);
		cout << "\t" << now() << " : " << "Flat Multiplying Started" << endl;

		time = FlatCMultiply(A._flat, B._flat, C._flat);


		cout << "\tTime: " << time << endl;

		if (VERIFY) {
			A.SaveToMatrix();
			B.SaveToMatrix();
			C.SaveToMatrix();
			output = string(" Method ") + Method + string(" - Phase 3 : Matrix Verifying ");
			prints(output, "#", 100);
			cout << "\tResult is :" << (VerifyMultiplication(A._matrix, B._matrix, C._matrix) ? " Verified" : " Wrong") << endl;
		}
	}
	return 0;
}