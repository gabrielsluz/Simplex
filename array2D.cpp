#include "array2D.hpp"
#include <iostream>


void array2D::initialize(int rows, int columns){
  _numRows = rows;
  _numColumns = columns;

  _a = allocArray();
}

float** array2D::allocArray(){
  float** m = new float*[_numRows];

  for(int i = 0; i < _numRows; i++){
    m[i] = new float[_numColumns];
    for(int j = 0; j < _numColumns; j++){
      m[i][j] = 0;
    }
  }
  return m;
}

void array2D::freeArray(){
  for(int i = 0; i < _numRows; i++){
    delete [] _a[i];
  }

  delete [] _a;
}

void array2D::setElement(int row, int column, float value){
  if(row >= _numRows || column >= _numColumns){
    std::cout << "Error, dimensoes erradas" << row << "-" << column << std::endl;
    return;
  }
  _a[row][column] = value;
}

float array2D::getElement(int row, int column){
  if(row >= _numRows || column >= _numColumns){
    std::cout << "Error, dimensoes erradas" << row << "-" << column << std::endl;
    return 0;
  }
  return _a[row][column];
}

void array2D::printElement(int row, int column){
  if(row >= _numRows || column >= _numColumns){
    std::cout << "Error, dimensoes erradas" << row << "-" << column << std::endl;
    return;
  }
  std::cout << _a[row][column];
}
