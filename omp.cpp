#include "matrix.h"
#include "omp.h"

// Initial Variables
int THREAD_ID;
int NUMBER_OF_THREADS = 8;
string PARALLEL_TYPE = "OPEN MP";

//Row Major Multiply
void  FlatRMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	auto pre = T::now();
	int IB, JB, KB;
#pragma omp parallel for private(IB, JB, KB)
	for (int i = 0; i < DIM; i++)
	{
		IB = i * DIM;
		for (int j = 0; j < DIM; j++)
		{
			JB = j * DIM;
			for (int k = 0; k < DIM; k++)
			{
				KB = k * DIM;
				final[IB + j] += left[IB + k] * right[KB + j];
			}
		}
	}
	auto finish = T::now();
	cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
	cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << endl;
}

//Column Major Multiply
void  FlatCMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	auto pre = T::now();
	int IB, JB, KB;
#pragma omp parallel for private(IB, JB, KB)
	for (int i = 0; i < DIM; i++)
	{
		IB = i * DIM;
		for (int j = 0; j < DIM; j++)
		{
			JB = j * DIM;
			for (int k = 0; k < DIM; k++)
			{
				KB = k * DIM;
				final[IB + j] += left[IB + k] * right[JB + k];
			}
		}
	}
	auto finish = T::now();
	cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
	cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << endl;
}


int main(int argc, char** argv) {
	omp_set_num_threads(NUMBER_OF_THREADS);
	string output;


	if (argc != 2) {
		cout << "ERROR: Please Specify Method: " << endl;
		cout << "e.g: ./a.out B";
		help(2);
		exit(EXIT_FAILURE);
	}
	help(2, 1);

	Matrix A;
	Matrix B;
	Matrix C;


	output = string(" Phase 1 : Matrix Creation ");
	prints(output, "#", 100);
	A.Init(SampleA1(), Matrix::ALL_RANDOM, true);
	if (string(argv[1]) == "E")
		B.Init(SampleA2(), Matrix::ALL_RANDOM, false);
	else
		B.Init(SampleA2(), Matrix::ALL_RANDOM, true);
	C.Init(NULL, Matrix::ALL_ZERO, true);


	// Method B
	if (string(argv[1]) == "B") {
		string output = string(" Phase 2 : Matrix Multiplying ");
		prints(output, "#", 100);

		FlatRMultiply(A._flat, B._flat, C._flat);


		if (VERIFY) {
			A.SaveToMatrix();
			B.SaveToMatrix();
			C.SaveToMatrix();
			output = string(" Phase 3 : Matrix Verifying ");
			prints(output, "#", 100);
			bool status = VerifyMultiplication(A._matrix, B._matrix, C._matrix);
			cout << "\tResult is :" << (status ? " Verified" : " Wrong") << endl;
		}
	}


	// Method E
	if (string(argv[1]) == "E") {
		output = string(" Phase 2 : Matrix Multiplying ");
		prints(output, "#", 100);
		cout << "\t" << now() << " : " << "Flat Multiplying Started" << endl;

		FlatCMultiply(A._flat, B._flat, C._flat);

		if (VERIFY) {
			A.SaveToMatrix();
			B.SaveToMatrix();
			C.SaveToMatrix();
			output = string(" Phase 3 : Matrix Verifying ");
			prints(output, "#", 100);
			bool status = VerifyMultiplication(A._matrix, B._matrix, C._matrix);
			cout << "\tResult is :" << (status ? " Verified" : " Wrong") << endl;
		}
	}
	return 0;
}