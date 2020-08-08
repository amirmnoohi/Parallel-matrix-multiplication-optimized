#include "matrix.h"
void help(int filter, int show_stats) {
	prints(" Help ", "#", 100);

	if (filter == 0 || filter == 1)
		cout << "Method A - FlatRMultiply() : Flat Multiplication if B is Row Major in SEQUENTIAL Mode" << endl;
	if (filter == 0 || filter == 2)
		cout << "Method B - FlatRMultiply() : Flat Multiplication if B is Row Major in OPENMP Mode" << endl;
	if (filter == 0 || filter == 3)
		cout << "Method C - FlatRMultiply() : Flat Multiplication if B is Row Major in MPI Mode" << endl;

	if (filter == 0 || filter == 1)
		cout << "Method D - FlatCMultiply() : Flat Multiplication if B is Column Major in SEQUENTIAL Mode" << endl;
	if (filter == 0 || filter == 2)
		cout << "Method E - FlatCMultiply() : Flat Multiplication if B is Column Major in OPENMP Mode" << endl;
	if (filter == 0 || filter == 3)
		cout << "Method F - FlatCMultiply() : Flat Multiplication if B is Column Major in MPI Mode" << endl;

	if (filter == 0 || filter == 1)
		cout << "Method G - BlockRMultiply() : Block Multiplication if B is Row Major in SEQUENTIAL Mode" << endl;
	if (filter == 0 || filter == 2)
		cout << "Method H - BlockRMultiply() : Block Multiplication if B is Row Major in OPENMP Mode" << endl;
	if (filter == 0 || filter == 3)
		cout << "Method I - BlockRMultiply() : Block Multiplication if B is Row Major in MPI Mode" << endl;

	if (filter == 0 || filter == 1)
		cout << "Method J - BlockCMultiply() : Block Multiplication if B is Column Major in SEQUENTIAL Mode" << endl;
	if (filter == 0 || filter == 2)
		cout << "Method K - BlockCMultiply() : Block Multiplication if B is Column Major in OPENMP Mode" << endl;
	if (filter == 0 || filter == 3)
		cout << "Method L - BlockCMultiply() : Block Multiplication if B is Column Major in MPI Mode" << endl;

	if (filter == 0 || show_stats == 1) {
		prints(" Phase 0 : Prechecking System Status ", "#", 100);
		cout << "\tParallel Type : " << PARALLEL_TYPE << endl;
		cout << "\tNumber of Threads : " << (filter == 1 ? 1 : NUMBER_OF_THREADS) << endl;
		cout << "\tMatrix Dimension : " << DIM << endl;
	}

}
bool VerifyMultiplication(int** left, int** right, int** result) {
	int temp;
	bool flag = true;
	cout << "\t" << now() << " : " << "Verify Multiplication Started" << endl;
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			temp = 0;
			for (int k = 0; k < DIM; k++)
			{
				temp += left[i][k] * right[k][j];
			}
			if (temp != result[i][j])
				flag = false;
		}
	}
	cout << "\t" << now() << " : " << "Verify Multiplication Finished" << endl;
	if (flag)
		return true;
	return false;
}
string now()
{
	time_t now = time(0);
	string dt = ctime(&now);
	dt.pop_back();
	return dt;
}
void prints(string text, const char* ch, int max_size = 20) {

	int number = ((max_size - (int)text.size()) / 2);
	for (int i = 0; i < number; i++)
	{
		text.insert(0, ch);
		text.push_back(ch[0]);
	}
	cout << endl << ((text.size() % 2) ? ch : "") << text << endl;
}
int** SampleA(int repeat) {
	int Dim = repeat * 8;
	int** temp = new int* [Dim];
	int start = 1;
	for (int i = 0; i < Dim; i++)
	{
		temp[i] = new int[Dim];
		for (int j = 0; j < Dim; j++)
		{
			temp[i][j] = start;
			start++;
			if (start == 9)
				start = 1;
		}
		start++;
		if (start == 9)
			start = 1;
	}
	return 0;
}
int** SampleB(int repeat) {
	int Dim = repeat * 8;
	int** temp = new int* [Dim];
	int start = 8;
	for (int i = 0; i < Dim; i++)
	{
		temp[i] = new int[Dim];
		for (int j = 0; j < Dim; j++)
		{
			temp[i][j] = start;
			start--;
			if (start == 0)
				start = 8;
		}
		start++;
		if (start == 0)
			start = 8;
	}
	return 0;
}
void Matrix::Init(int** matrix = NULL, short int type = ALL_RANDOM, bool is_row_major = true)
{
	cout << "\t" << now() << " : " << "Creating Matrix Started" << endl;
	_is_row_major = is_row_major;
	int temp = 0;
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			if (type == ALL_MATRIX)
				temp = matrix[i][j];
			if (type == ALL_RANDOM)
				temp = rand() % MAX_VAL + MIN_VAL;
			if (is_row_major)
				_flat[i * DIM + j] = temp;
			else
				_flat[j * DIM + i] = temp;
		}
	}
	cout << "\t" << now() << " : " << "Creating Matrix Finished" << endl;
}
Matrix::Matrix()
{
	_matrix = nullptr;
	_flat = new int[DIM * DIM];
	_is_row_major = true;
}
void Matrix::Clear()
{
	if (_matrix != nullptr) {
		for (int i = 0; i < DIM; i++)
		{
			delete[] _matrix[i];
		}
		delete[] _matrix;
	}
	delete[] _flat;
}
Matrix::~Matrix()
{
	if (_matrix != nullptr) {
		for (int i = 0; i < DIM; i++)
		{
			delete[] _matrix[i];
		}
		delete[] _matrix;
	}
	delete[] _flat;
}
void Matrix::MatrixShow()
{
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			if (_is_row_major)
				printf("%-8d", _flat[i * DIM + j]);
			else
				printf("%-8d", _flat[j * DIM + i]);
		}
		cout << endl;
	}
}
void Matrix::SaveToMatrix()
{
	_matrix = new int* [DIM];
	for (int i = 0; i < DIM; i++)
	{
		_matrix[i] = new int[DIM];
		for (int j = 0; j < DIM; j++)
		{
			if (_is_row_major)
				_matrix[i][j] = _flat[i * DIM + j];
			else
				_matrix[i][j] = _flat[j * DIM + i];

		}
	}
}
void Matrix::FlatShow()
{
	for (int i = 0; i < DIM * DIM; i++)
		cout << _flat[i] << " \n"[i == DIM * DIM - 1];
}