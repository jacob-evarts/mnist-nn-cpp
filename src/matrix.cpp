// Code was inspired from this site: https://causeyourestuck.io/category/network-en/ 

#include "../include/matrix.h"

Matrix::Matrix() {

}

Matrix::Matrix(int height, int width) {
	this->height = height;
	this->width = width;
    // creates a 2d vector of size height, with each entry being a vector of size width
	this->array = vector<vector<double>>(height, vector<double>(width));			
}

Matrix::Matrix(vector<vector<double>> const &array) {
	if (array.size() == 0) {
		cout << "array size is 0";
	}
	else {
        // width is the same as the length of any entry in the array
		this->width = array[0].size();									
		this->height = array.size();
		this->array = array;
	}
}

Matrix Matrix::multiply(double const &value) {
	Matrix returnMat = Matrix(height, width);
	int hCtr;
	int wCtr;

	for (hCtr = 0; hCtr < height; hCtr++) {
		for (wCtr = 0; wCtr < width; wCtr++) {
            // multiplies every value of the matrix by the given value
			returnMat.array[hCtr][wCtr] = array[hCtr][wCtr] * value;			
		}
	}
	return returnMat;
}

Matrix Matrix::multiply(Matrix const &mat) const{
	Matrix returnMat = Matrix(height, width);
    // if these are not the same there may be an out of bounds error
	if (height != mat.height || width != mat.width) {
		cerr << "height and widths are not the same, cannot compute multiplication of matrices" << endl;							
		exit(1);
	}
	else {
		int hCtr;
		int wCtr;

		for (hCtr = 0; hCtr < height; hCtr++) {
			for (wCtr = 0; wCtr < width; wCtr++) {
                // multiplies each element of the matrix by the corresponding element in the given matrix
				returnMat.array[hCtr][wCtr] = array[hCtr][wCtr] * mat.array[hCtr][wCtr];		
			}
		}
	}

	return returnMat;
}

Matrix Matrix::add(Matrix const &mat) const {
	Matrix returnMat = Matrix(height, width);
	if (height != mat.height || width != mat.width) {
        // if these are not the same there may be an out of bounds error
		cout << "height and widths are not the same, cannot compute addition" << endl;						
		exit(1);
	}
	else {
		int hCtr;
		int wCtr;

		for (hCtr = 0; hCtr < height; hCtr++) {
			for (wCtr = 0; wCtr < width; wCtr++) {
                // adds each element of the matrix by the corresponding element in the given matrix
				returnMat.array[hCtr][wCtr] = array[hCtr][wCtr] + mat.array[hCtr][wCtr];		
			}
		}
	}
	return returnMat;
}

Matrix Matrix::subtract(Matrix const &mat) const {
	Matrix returnMat = Matrix(height, width);
    // if these are not the same there may be an out of bounds error
	if (height != mat.height || width != mat.width) {
		cout << "height and widths are not the same, cannot compute subtraction" << endl;
		exit(1);													
	}
	int hCtr;
	int wCtr;

	for (hCtr = 0; hCtr < height; hCtr++) {
		for (wCtr = 0; wCtr < width; wCtr++) {
            // subtracts each element of the matrix by the corresponding element in the given matrix
			returnMat.array[hCtr][wCtr] = array[hCtr][wCtr] - mat.array[hCtr][wCtr];		
		}
	}
	return returnMat;
}

Matrix Matrix::dot(Matrix const &mat) const {
	int hCtr;
    // mat width counter
	int mwCtr;												
	int wCtr;
    // need this to calculate the value of each index before it is added
	double tempDotProd = 0;									
	int matWidth = mat.width;

	Matrix returnMat = Matrix(height, matWidth);
    // width needs to match the other matrices height for dot product to work
	if (width != mat.height) {								
		cout << "height and widths are not the same, cannot compute dot product" << endl;
		exit(1);
	}

	for (hCtr = 0; hCtr < height; hCtr++) {
		for (mwCtr = 0; mwCtr < matWidth; mwCtr++) {
			for (wCtr = 0; wCtr < width; wCtr++) {
                // computes the tempDotProd to later be placed in the matrix
				tempDotProd += array[hCtr][wCtr] * mat.array[wCtr][mwCtr];		
			}

			returnMat.array[hCtr][mwCtr] = tempDotProd;
            // reset so it can be used for the next value
			tempDotProd = 0;													
		}
	}

	return returnMat;
}

Matrix Matrix::transpose() const {
	Matrix returnMat = Matrix(width, height);
	int hCtr;
	int wCtr;

	for (wCtr = 0; wCtr < width; wCtr++) {
		for (hCtr = 0; hCtr < height; hCtr++) {
            // transposes each element of the matrix by the corresponding element in the given matrix
			returnMat.array[wCtr][hCtr] = array[hCtr][wCtr];			
		}
	}
	return returnMat;
}

// applies any function that takes and returns a double to a matrix
Matrix Matrix::applyFunction(double(*function)(double)) const {			
	Matrix returnMat = Matrix(height, width);
	int hCtr;
	int wCtr;
	for (hCtr = 0; hCtr < height; hCtr++) {
		for (wCtr = 0; wCtr < width; wCtr++) {
            // applies the function on each index
			returnMat.array[hCtr][wCtr] = (*function)(array[hCtr][wCtr]);			
		}
	}

	return returnMat;
}

int Matrix::getWidth() {			
	return width;
}

int Matrix::getHeight() {			
	return height;
}

vector<vector<double>> Matrix::getArray() {	
	return array;
}

void Matrix::setArray(int hInd, int wInd, double value) {
    array[hInd][wInd] = value;
}

void Matrix::print() const {
	int hCtr;
	int wCtr;

	for (hCtr = 0; hCtr < height; hCtr++) {
		for (wCtr = 0; wCtr < width; wCtr++) {
            // prints each index
			cout << array[hCtr][wCtr];				
			cout << " ";
		}
        // new line to separate rows
		cout << endl;								
	}
}

// overloads input operator so you can print a matrix. cout << matrix
ostream& operator<<(ostream &s, const Matrix &mat) {			
	mat.print();
	return s;
}

// overloads input operator so you can add matrices. 
Matrix operator+(const Matrix &mat1, const Matrix &mat2) {
	Matrix returnMat = mat1.add(mat2);
	return returnMat;
}

// overloads input operator so you can subtract matrices. 
Matrix operator-(const Matrix &mat1, const Matrix &mat2) {
	Matrix returnMat = mat1.subtract(mat2);
	return returnMat;
}
