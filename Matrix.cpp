#include "Matrix.hpp"
#include <iostream>


Matrix::Matrix(int numRows, int numColumns){
  _numRows = numRows;
  _numColumns = numColumns;

  _matrix = new float*[numRows];

  for(int i = 0; i < numRows; i++){
    _matrix[i] = new float[numColumns];
  }
}

Matrix::~Matrix(){
  for(int i = 0; i < _numRows; i++){
    delete [] _matrix[i];
  }

  delete [] _matrix;
}

void Matrix::setElement(int row, int column, float value){
  if(row >= _numRows || column >= _numColumns){
    std::cout << "Error, dimensoes erradas" << std::endl;
    return;
  }
  _matrix[row][column] = value;
}

void Matrix::printMatrix(){
  
}
