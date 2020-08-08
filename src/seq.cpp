#include "matrix.h"
#include "omp.h"

// Initial Variables
int THREAD_ID;
int NUMBER_OF_THREADS = 32;
string PARALLEL_TYPE = "SEQUENTIAL";

// Flat Row Major Multiply
void  FlatRMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	auto pre = T::now();
	int IB, JB, KB;
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
	cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
}
// Flat Column Major Multiply
void  FlatCMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	auto pre = T::now();
	int IB, JB, KB;
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
	cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
}
// Block Row Major Multiply
void BlockRMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	auto pre = T::now();

	int i = 0, j = 0, k = 0, jj = 0, kk = 0;
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
	string output;


	if (argc != 2) {
		cout << "ERROR: Please Specify Method: " << endl;
		cout << "e.g: ./a.out A";
		help(1);
		exit(EXIT_FAILURE);
	}
	help(1, 1);

	Matrix A;
	Matrix B;
	Matrix C;


	output = string(" Phase 1 : Matrix Creation ");
	prints(output, "#", 100);
	A.Init(NULL, Matrix::ALL_RANDOM, true);
	if (string(argv[1]) == "D" || string(argv[1]) == "J")
		B.Init(NULL, Matrix::ALL_RANDOM, false);
	else
		B.Init(NULL, Matrix::ALL_RANDOM, true);
	C.Init(NULL, Matrix::ALL_ZERO, true);

	// Method A
	if (string(argv[1]) == "A") {
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
	// Method D
	if (string(argv[1]) == "D") {
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
	}
	// Method G
	if (string(argv[1]) == "G") {
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
	// Method J
	if (string(argv[1]) == "J") {
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