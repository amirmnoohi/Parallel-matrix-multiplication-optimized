#include "matrix.h"
#include "omp.h"

// Initial Variables
int THREAD_ID;
int NUMBER_OF_THREADS = 4;
string PARALLEL_TYPE = "OPEN MP";

//Row Major Multiply
void  FlatRMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	auto pre = T::now();
	#pragma omp parallel for collapse(2)
	for (int i = 0; i < DIM; i++)
		for (int j = 0; j < DIM; j++)
			for (int k = 0; k < DIM; k++)
				final[i * DIM + j] += left[i * DIM + k] * right[k * DIM + j];

	auto finish = T::now();
	cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
	cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
}

//Column Major Multiply
void  FlatCMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	auto pre = T::now();
	#pragma omp parallel for collapse(2)
	for (int i = 0; i < DIM; i++)
		for (int j = 0; j < DIM; j++)
			for (int k = 0; k < DIM; k++)
				final[i * DIM + j] += left[i * DIM + k] * right[j * DIM + k];

	auto finish = T::now();
	cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
	cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
}

// Block Row Major Multiply
void BlockRMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	auto pre = T::now();

	int i = 0, j = 0, k = 0, jj = 0, kk = 0;
	#pragma omp parallel for collapse(2)
	for (int i = 0; i < DIM; i += 2)
	{
		for (int j = 0; j < DIM; j += 2)
		{
			for (int k = 0; k < DIM; k += 2)
			{
				final[i * DIM + j] += left[i * DIM + k] * right[k * DIM + j] + left[i * DIM + k + 1] * right[(k + 1) * DIM + j];
				final[i * DIM + j + 1] += left[i * DIM + k] * right[k * DIM + j + 1] + left[i * DIM + k + 1] * right[(k + 1) * DIM + j + 1];
				final[(i + 1) * DIM + j] += left[(i + 1) * DIM + k] * right[k * DIM + j] + left[(i + 1) * DIM + k + 1] * right[(k + 1) * DIM + j];
				final[(i + 1) * DIM + j + 1] += left[(i + 1) * DIM + k] * right[k * DIM + j + 1] + left[(i + 1) * DIM + k + 1] * right[(k + 1) * DIM + j + 1];
			}
		}
	}
	auto finish = T::now();
	cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
	cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
}

// Block Column Major Multiply
void BlockCMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	auto pre = T::now();

	int i = 0, j = 0, k = 0, jj = 0, kk = 0;
	#pragma omp parallel for collapse(2)
	for (int i = 0; i < DIM; i += 2)
	{
		for (int j = 0; j < DIM; j += 2)
		{
			for (int k = 0; k < DIM; k += 2)
			{
				final[i * DIM + j] += left[i * DIM + k] * right[j * DIM + k] + left[i * DIM + k + 1] * right[j * DIM + k + 1];
				final[i * DIM + j + 1] += left[i * DIM + k] * right[(j + 1) * DIM + k] + left[i * DIM + k + 1] * right[(j + 1) * DIM + k + 1];
				final[(i + 1) * DIM + j] += left[(i + 1) * DIM + k] * right[j * DIM + k] + left[(i + 1) * DIM + k + 1] * right[j * DIM + k + 1];
				final[(i + 1) * DIM + j + 1] += left[(i + 1) * DIM + k] * right[(j + 1) * DIM + k] + left[(i + 1) * DIM + k + 1] * right[(j + 1) * DIM + k + 1];
			}
		}
	}
	auto finish = T::now();
	cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
	cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
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
	A.Init(SampleA(1), Matrix::ALL_MATRIX, true);
	if (string(argv[1]) == "E" || string(argv[1]) == "K")
		B.Init(SampleB(1), Matrix::ALL_MATRIX, false);
	else
		B.Init(SampleB(1), Matrix::ALL_MATRIX, true);
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
		C.MatrixShow();
	}
	
	// Method E
	if (string(argv[1]) == "E") {
		output = string(" Phase 2 : Matrix Multiplying ");
		prints(output, "#", 100);

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
		C.MatrixShow();
	}

	// Method H
	if (string(argv[1]) == "H") {
		output = string(" Phase 2 : Matrix Multiplying ");
		prints(output, "#", 100);

		BlockRMultiply(A._flat, B._flat, C._flat);

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
	// Method K
	if (string(argv[1]) == "K") {
		output = string(" Phase 2 : Matrix Multiplying ");
		prints(output, "#", 100);

		BlockCMultiply(A._flat, B._flat, C._flat);

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