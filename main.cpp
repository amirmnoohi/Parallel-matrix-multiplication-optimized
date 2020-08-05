#include "lib.h"
int main(int argc, char ** argv) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &workers);
	MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);

	



	MPI_Finalize();
	return 0;
}