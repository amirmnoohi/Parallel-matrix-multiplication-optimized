#include "matrix.h"
#include "mpi.h"

// Initial Variables
int THREAD_ID;
int NUMBER_OF_THREADS;
string PARALLEL_TYPE = "MPI";

// Flat Row Major Multiply
void  FlatRMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	int start, end, temp = 0, pos = 0;
	start = THREAD_ID * (DIM / NUMBER_OF_THREADS);
	end = start + (DIM / NUMBER_OF_THREADS);
	int* result = new int[DIM * DIM / NUMBER_OF_THREADS];
	auto pre = T::now();
	if (!THREAD_ID) {
		cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	}
	for (int i = start; i < end; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			temp = 0;
			for (int k = 0; k < DIM; k++)
			{
				temp += left[i * DIM + k] * right[k * DIM + j];
			}
			result[pos] = temp;
			pos++;
		}
	}
	MPI_Gather(result, DIM * DIM / NUMBER_OF_THREADS, MPI_INT, final, DIM * DIM / NUMBER_OF_THREADS, MPI_INT, 0, MPI_COMM_WORLD);
	if (!THREAD_ID) {
		auto finish = T::now();
		cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
		cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
	}
	else {
		delete[] result;
	}
}
// Flat Column Major Multiply
void  FlatCMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	int start, end, temp = 0, pos = 0;
	start = THREAD_ID * (DIM / NUMBER_OF_THREADS);
	end = start + (DIM / NUMBER_OF_THREADS);
	int* result = new int[DIM * DIM / NUMBER_OF_THREADS];
	auto pre = T::now();
	if (!THREAD_ID) {
		cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	}
	for (int i = start; i < end; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			temp = 0;
			for (int k = 0; k < DIM; k++)
			{
				temp += left[i * DIM + k] * right[j * DIM + k];
			}
			result[pos] = temp;
			pos++;
		}
	}
	MPI_Gather(result, DIM * DIM / NUMBER_OF_THREADS, MPI_INT, final, DIM * DIM / NUMBER_OF_THREADS, MPI_INT, 0, MPI_COMM_WORLD);
	if (!THREAD_ID) {
		auto finish = T::now();
		cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
		cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
	}
	else {
		delete[] result;
	}
}
// Block Row Major Multiply
void BlockRMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	int start, end, temp[4] = { 0 }, pos = 0;
	start = THREAD_ID * (DIM / NUMBER_OF_THREADS);
	end = start + (DIM / NUMBER_OF_THREADS);
	int* result = new int[DIM * (DIM / NUMBER_OF_THREADS)];
	auto pre = T::now();
	if (!THREAD_ID) {
		cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	}

	for (int i = start; i < end; i += 2)
	{
		for (int j = 0; j < DIM; j += 2)
		{
			memset(temp, 0, 16);
			for (int k = 0; k < DIM; k += 2)
			{
				temp[0] += left[i * DIM + k] * right[k * DIM + j] + left[i * DIM + k + 1] * right[(k + 1) * DIM + j];
				temp[1] += left[i * DIM + k] * right[k * DIM + j + 1] + left[i * DIM + k + 1] * right[(k + 1) * DIM + j + 1];
				temp[2] += left[(i + 1) * DIM + k] * right[k * DIM + j] + left[(i + 1) * DIM + k + 1] * right[(k + 1) * DIM + j];
				temp[3] += left[(i + 1) * DIM + k] * right[k * DIM + j + 1] + left[(i + 1) * DIM + k + 1] * right[(k + 1) * DIM + j + 1];
			}
			result[pos] = temp[0];
			result[pos + 1] = temp[1];
			result[pos + DIM] = temp[2];
			result[pos + DIM + 1] = temp[3];
			pos += 2;
		}
		pos += DIM;
	}
	MPI_Gather(result, DIM * DIM / NUMBER_OF_THREADS, MPI_INT, final, DIM * DIM / NUMBER_OF_THREADS, MPI_INT, 0, MPI_COMM_WORLD);
	if (!THREAD_ID) {
		auto finish = T::now();
		cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
		cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
	}
}
// Block Column Major Multiply
void BlockCMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	int start, end, temp[4] = { 0 }, pos = 0;
	start = THREAD_ID * (DIM / NUMBER_OF_THREADS);
	end = start + (DIM / NUMBER_OF_THREADS);
	int* result = new int[(DIM / NUMBER_OF_THREADS) * DIM];
	auto pre = T::now();
	if (!THREAD_ID) {
		cout << "\t" << now() << " : " << "Multiplying Started" << endl;
	}

	for (int i = start; i < end; i += 2)
	{
		for (int j = 0; j < DIM; j += 2)
		{
			memset(temp, 0, 16);
			for (int k = 0; k < DIM; k += 2)
			{
				temp[0] += left[i * DIM + k] * right[j * DIM + k] + left[i * DIM + k + 1] * right[j * DIM + k + 1];
				temp[1] += left[i * DIM + k] * right[(j + 1) * DIM + k] + left[i * DIM + k + 1] * right[(j + 1) * DIM + k + 1];
				temp[2] += left[(i + 1) * DIM + k] * right[j * DIM + k] + left[(i + 1) * DIM + k + 1] * right[j * DIM + k + 1];
				temp[3] += left[(i + 1) * DIM + k] * right[(j + 1) * DIM + k] + left[(i + 1) * DIM + k + 1] * right[(j + 1) * DIM + k + 1];
			}
			result[pos] = temp[0];
			result[pos + 1] = temp[1];
			result[pos + DIM] = temp[2];
			result[pos + DIM + 1] = temp[3];
			pos += 2;
		}
		pos += DIM;
	}
	MPI_Gather(result, DIM * DIM / NUMBER_OF_THREADS, MPI_INT, final, DIM * DIM / NUMBER_OF_THREADS, MPI_INT, 0, MPI_COMM_WORLD);
	if (!THREAD_ID) {
		auto finish = T::now();
		cout << "\t" << now() << " : " << "Multiplying Finished" << endl;
		cout << "\tTime: " << chrono::duration_cast<Time>(finish - pre).count() << Time_Unit << endl;
	}
}

int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &NUMBER_OF_THREADS);
	MPI_Comm_rank(MPI_COMM_WORLD, &THREAD_ID);
	string output;

	if (THREAD_ID == 0) {
		if (argc != 2) {
			cout << "ERROR: Please Specify Method: " << endl;
			cout << "e.g: mpicc -np 4 a.out C";
			help(3);
			exit(EXIT_FAILURE);
		}
		if (DIM % NUMBER_OF_THREADS != 0) {
			cout << "ERROR: Matrix can not be calculated with this number of tasks.\n";
			exit(EXIT_FAILURE);
		}
		if (string(argv[1]) == "I" || string(argv[1]) == "L") {
			if (DIM  / NUMBER_OF_THREADS < 4) {
				cout << "ERROR: Matrix can not be calculated with this number of tasks.\n";
				exit(EXIT_FAILURE);
			}
		}
		help(3, 1);
	}

	Matrix A;
	Matrix B;
	Matrix C;


	if (!THREAD_ID) {
		output = string(" Phase 1 : Matrix Creation ");
		prints(output, "#", 100);
		A.Init(SampleA(1), Matrix::ALL_MATRIX, true);
		if (string(argv[1]) == "F" || string(argv[1]) == "L")
			B.Init(SampleB(1), Matrix::ALL_MATRIX, false);
		else
			B.Init(SampleB(1), Matrix::ALL_MATRIX, true);
		C.Init(NULL, Matrix::ALL_ZERO, true);
	}

	MPI_Bcast(A._flat, DIM * DIM, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(B._flat, DIM * DIM, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(C._flat, DIM * DIM, MPI_INT, 0, MPI_COMM_WORLD);

	// Method C
	if (string(argv[1]) == "C") {
		if (!THREAD_ID) {
			string output = string(" Phase 2 : Matrix Multiplying ");
			prints(output, "#", 100);
		}

		FlatRMultiply(A._flat, B._flat, C._flat);

		if (!THREAD_ID && VERIFY) {
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
	// Method F
	if (string(argv[1]) == "F") {
		if (!THREAD_ID) {
			output = string(" Phase 2 : Matrix Multiplying ");
			prints(output, "#", 100);
		}

		FlatCMultiply(A._flat, B._flat, C._flat);

		if (!THREAD_ID && VERIFY) {
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
	// Method I
	if (string(argv[1]) == "I") {
		if (!THREAD_ID) {
			output = string(" Phase 2 : Matrix Multiplying ");
			prints(output, "#", 100);
		}

		BlockRMultiply(A._flat, B._flat, C._flat);

		if (!THREAD_ID && VERIFY) {
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
	// Method L
	if (string(argv[1]) == "L") {
		if (!THREAD_ID) {
			output = string(" Phase 2 : Matrix Multiplying ");
			prints(output, "#", 100);
		}

		BlockCMultiply(A._flat, B._flat, C._flat);

		if (!THREAD_ID && VERIFY) {
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
	MPI_Finalize();
	return 0;
}