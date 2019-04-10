#include "Matrix.hpp"
#include <iostream>


Matrix::Matrix(int restrictions, int variables){
  _numRows = restrictions + 1;
  _numColumns = restrictions + variables + restrictions + 1;
  _restrictions = restrictions;
  _variables = variables;

  _matrix = new float*[_numRows];

  for(int i = 0; i < _numRows; i++){
    _matrix[i] = new float[_numColumns];
    for(int j = 0; j < _numColumns; j++){
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
    std::cout << "Error, dimensoes erradas" << row << "-" << column << std::endl;
    return;
  }
  _matrix[row][column] = value;
}


void Matrix::setVeroTM(){
  for(int i=0; i<=_restrictions; i++){
    for(int j=0; j<_restrictions; j++){
      if((i-1) == j && i !=0){
        //_matrix[i][j] = 1;
        setElement(i,j,1);
      }
      else{
        //_matrix[i][j] = 0;
        setElement(i,j,0);
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



int Matrix::Simplex(){
  for(int i = 0; i <_restrictions; i++ ){
    if(_matrix[i+1][_numColumns-1] < 0){
      return Dual();
    }
  }
  return Primal();
}

int Matrix::Primal(){
  std::cout << "Primal" << std::endl;
  return 0;
}

int Matrix::Dual(){
  std::cout << "Dual" << std::endl;
  return 0;
}

void Matrix::printResult(int result){

}
