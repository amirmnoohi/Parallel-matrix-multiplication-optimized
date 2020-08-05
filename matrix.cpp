#include "matrix.h"

bool VerifyMultiplication(T** fmatrix, T** smatrix, T** rmatrix) {
	T temp;
	bool flag = true;
	cout << "\t" << now() << " : " << "Verify Multiplication Started" << endl;
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			temp = 0;
			for (int k = 0; k < DIM; k++)
			{
				temp += fmatrix[i][k] * smatrix[k][j];
			}
			if (temp != rmatrix[i][j])
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
T** SampleA() {
	T b[8][8] = {
		{8,7,6,5,4,3,2,1},
		{7,6,5,4,3,2,1,8},
		{6,5,4,3,2,1,8,7},
		{5,4,3,2,1,8,7,6},
		{4,3,2,1,8,7,6,5},
		{3,2,1,8,7,6,5,4},
		{2,1,8,7,6,5,4,3},
		{1,8,7,6,5,4,3,2}
	};
	T** temp = new T * [8];
	for (int i = 0; i < 8; i++)
	{
		temp[i] = new T[8];
		for (int j = 0; j < 8; j++)
		{
			temp[i][j] = b[i][j];
		}
	}
	return temp;
}
T** SampleB() {
	T a[8][8] =
	{
		{1,2,3,4,5,6,7,8},
		{2,3,4,5,6,7,8,1},
		{3,4,5,6,7,8,1,2},
		{4,5,6,7,8,1,2,3},
		{5,6,7,8,1,2,3,4},
		{6,7,8,1,2,3,4,5},
		{7,8,1,2,3,4,5,6},
		{8,1,2,3,4,5,6,7}
	};
	T** temp = new T * [8];
	for (int i = 0; i < 8; i++)
	{
		temp[i] = new T[8];
		for (int j = 0; j < 8; j++)
		{
			temp[i][j] = a[i][j];
		}
	}
	return temp;
}
Matrix::Matrix(T** matrix = NULL, short int type = ALL_RANDOM, bool is_row_major = true)
/*
	matrix : feeding inner matrix
	type   : (-1 => from matrix) - (0 => all zero) - (1 => random)
*/
{
	cout << "\t" << now() << " : " << "Creating Matrix Started" << endl;
	_is_row_major = is_row_major;
	_matrix = new T * [DIM];
	_flat = new T[DIM * DIM];

	for (int i = 0; i < DIM; i++) {
		_matrix[i] = new T[DIM];
		for (int j = 0; j < DIM; j++) {
			if (type == ALL_MATRIX)
				_matrix[i][j] = matrix[i][j];
			else if (type == ALL_ZERO)
				_matrix[i][j] = 0;
			else
				_matrix[i][j] = rand() % MAX_VAL + MIN_VAL;
			if (is_row_major)
				_flat[i * DIM + j] = _matrix[i][j];
			else
				_flat[j * DIM + i] = _matrix[i][j];
		}
	}
	cout << "\t" << now() << " : " << "Creating Matrix Finished" << endl;
}
void Matrix::clear()
{
	for (int i = 0; i < DIM; i++)
	{
		delete[] _matrix[i];
	}
	delete[] _matrix;
	delete[] _flat;
}
Matrix::~Matrix()
{
	for (int i = 0; i < DIM; i++)
	{
		delete[] _matrix[i];
	}
	delete[] _matrix;
	delete[] _flat;
}
void Matrix::MatrixShow()
/*
*/
{
	for (int i = 0; i < DIM; i++)
		for (int j = 0; j < DIM; j++)
			cout << _matrix[i][j] << " " << " \n"[j == DIM - 1];
}
void Matrix::FlatShow()
/*
*/
{
	for (int i = 0; i < DIM * DIM; i++)
		cout << _flat[i] << " \n"[i == DIM * DIM - 1];
}