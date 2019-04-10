#include "Matrix.hpp"
#include <iostream>


Matrix::Matrix(int numRows, int numColumns, int restrictions, int variables){
  _numRows = numRows;
  _numColumns = numColumns;
  _restrictions = restrictions;
  _variables = variables;

  _matrix = new float*[numRows];

  for(int i = 0; i < numRows; i++){
    _matrix[i] = new float[numColumns];
    for(int j = 0; j < numColumns; j++){
      _matrix[i][j] = 0;
    }
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


void Matrix::setVeroTM(){
  for(int i=0; i<=_restrictions; i++){
    for(int j=0; j<_restrictions; j++){
      if((i-1) == j && i !=0){
        _matrix[i][j] = 1;
      }
      else{
        _matrix[i][j] = 0;
      }
    }
  }
}


void Matrix::getVectorC(){
  int offset = _restrictions;
  float value = 0.0;

  for(int i=0; i<_variables; i++){
    std::cin >> value;
    _matrix[0][i+offset] = -value;
  }

  //Transforma em PLI
  offset = _restrictions + _variables;
  for(int i=0; i<_restrictions+1; i++){
    _matrix[0][i+offset] = 0;
  }
}

void Matrix::getAandB(){
  int offset = _restrictions;
  float value = 0.0;

  for(int i=0; i<_restrictions; i++){
    for(int j=0; j<_variables; j++){
      std::cin >> _matrix[i+1][j+offset];
    }
    std::cin >> _matrix[i+1][_numColumns-1];
  }

  //Transforma em PLI
  offset = _restrictions + _variables;
  for(int i=0; i<_restrictions; i++){
    for(int j=0; j<_variables; j++){
      if(i == j){
        _matrix[i+1][j+offset] = 1;
      }
      else{
        _matrix[i+1][j+offset] = 0;
      }
    }
  }

}


void Matrix::printMatrix(){
  for(int i =0; i< _numRows; i++){
    for(int j =0; j< _numColumns; j++){
      std::cout << _matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}
