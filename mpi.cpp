#include "matrix.h"
#include "mpi.h"

// Initial Variables
int THREAD_ID;
int NUMBER_OF_THREADS;
string PARALLEL_TYPE = "MPI";
string Method = "C";

// Flat Row Major Multiply
int_fast64_t  FlatRMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	int start, end, temp = 0, pos = 0;
	start = THREAD_ID * (DIM / NUMBER_OF_THREADS);
	end = start + (DIM / NUMBER_OF_THREADS);
	int* result = new int[DIM * DIM / NUMBER_OF_THREADS];
	auto pre = T::now();
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
	if (THREAD_ID)
		delete[] result;
	return  chrono::duration_cast<Time>(T::now() - pre).count();
}

// Flat Column Major Multiply
int_fast64_t  FlatCMultiply(int left[DIM * DIM], int right[DIM * DIM], int final[DIM * DIM]) {
	int start, end, temp = 0, pos = 0;
	start = THREAD_ID * (DIM / NUMBER_OF_THREADS);
	end = start + (DIM / NUMBER_OF_THREADS);
	int* result = new int[DIM * DIM / NUMBER_OF_THREADS];
	auto pre = T::now();
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
	if (THREAD_ID)
		delete[] result;
	return  chrono::duration_cast<Time>(T::now() - pre).count();
}


int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NUMBER_OF_THREADS);
	MPI_Comm_rank(MPI_COMM_WORLD, &THREAD_ID);
	string output;
	int_fast64_t time;

	if (THREAD_ID == 0) {
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
	}

	Matrix A;
	Matrix B;
	Matrix C;


	if (!THREAD_ID) {
		output = string(" Phase 1 : Matrix Creation ");
		prints(output, "#", 100);
		A.Init(SampleA1(), Matrix::ALL_RANDOM, true);
		if (argv[1] == "F")
			B.Init(SampleA2(), Matrix::ALL_RANDOM, false);
		else
			B.Init(SampleA2(), Matrix::ALL_RANDOM, true);
		C.Init(NULL, Matrix::ALL_ZERO, true);
	}

	MPI_Bcast(A._flat, DIM * DIM, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(B._flat, DIM * DIM, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(C._flat, DIM * DIM, MPI_INT, 0, MPI_COMM_WORLD);

	// Method C
	if (argv[1] == "C") {
		if (!THREAD_ID) {
			Method = "C";
			string output = string(" Method ") + Method + string(" - Phase 2 : Matrix Multiplying ");
			prints(output, "#", 100);
		}
		time = FlatRMultiply(A._flat, B._flat, C._flat);
		if (!THREAD_ID) {
			cout << "\tTime: " << time << endl;
		}
		if (!THREAD_ID && VERIFY) {
			A.SaveToMatrix();
			B.SaveToMatrix();
			C.SaveToMatrix();
			output = string(" Method ") + Method + string(" - Phase 3 : Matrix Verifying ");
			prints(output, "#", 100);
			cout << "\tResult is :" << (VerifyMultiplication(A._matrix, B._matrix, C._matrix) ? " Verified" : " Wrong") << endl;
		}
	}


	// Method F
	if (argv[1] == "F") {
		if (!THREAD_ID) {
			Method = "F";
			output = string(" Method ") + Method + string(" - Phase 2 : Matrix Multiplying ");
			prints(output, "#", 100);
			cout << "\t" << now() << " : " << "Flat Multiplying Started" << endl;
		}
		time = FlatCMultiply(A._flat, B._flat, C._flat);
		if (!THREAD_ID) {
			cout << "\t" << now() << " : " << "Flat Multiplying Finished" << endl;
			cout << "\tTime: " << time << endl;
		}
		if (!THREAD_ID && VERIFY) {
			A.SaveToMatrix();
			B.SaveToMatrix();
			C.SaveToMatrix();
			output = string(" Method ") + Method + string(" - Phase 3 : Matrix Verifying ");
			prints(output, "#", 100);
			cout << "\tResult is :" << (VerifyMultiplication(A._matrix, B._matrix, C._matrix) ? " Verified" : " Wrong") << endl;
		}
	}



	MPI_Finalize();
	return 0;
}