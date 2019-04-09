#include "Matrix.hpp"


Matrix::Matrix(int numRows, int numColumns){
  this->numRows = numRows;
  this->numColumns = numColumns;

  this->matrix = new float*[numRows];

  for(int i = 0; i < numRows; i++){
    this->matrix[i] = new float[numColumns];
  }
}
