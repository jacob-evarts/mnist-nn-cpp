#ifndef MATRIX
#define MATRIX

#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

class Matrix {
public:
	Matrix(vector<vector<double> > const &array);
	Matrix();
	Matrix(int height, int width);

	Matrix multiply(double const &value);
	Matrix multiply(Matrix const &num) const;
	Matrix add(Matrix const &num) const;							//for functions, constant tag goes at the end. Makes it read-only
	Matrix subtract(Matrix const &num) const;
	Matrix dot(Matrix const &num) const;
	Matrix transpose() const;
	Matrix applyFunction(double(*function)(double)) const;
	int getWidth();
	int getHeight();
	vector<vector<double>> getArray();
    void setArray(int hInd, int wInd, double value);
	void print() const;

	friend ostream& operator<<(ostream &s, const Matrix &mat);				//friend functions for overloading
	friend Matrix operator+(const Matrix &mat1, const Matrix &mat2);
	friend Matrix operator-(const Matrix &mat1, const Matrix &mat2);

private:
	vector<vector<double>> array;		//the "matrix"
	int width;
	int height;

};

#endif // !MATRIX
