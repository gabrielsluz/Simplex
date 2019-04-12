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
    //_matrix[0][i+offset] = -value;
    setElement(0,i+offset, -value);
  }

  //Transforma em PLI
  offset = _restrictions + _variables;
  for(int i=0; i<_restrictions+1; i++){
    //_matrix[0][i+offset] = 0;
    setElement(0,i+offset,0);
  }
}

void Matrix::getAandB(){
  int offset = _restrictions;
  float value = 0.0;

  for(int i=0; i<_restrictions; i++){
    for(int j=0; j<_variables; j++){
      //std::cin >> _matrix[i+1][j+offset];
      std::cin >> value;
      setElement(i+1,j+offset,value);
    }
    //std::cin >> _matrix[i+1][_numColumns-1];
    std::cin >> value;
    setElement(i+1,_numColumns-1,value);
  }

  //Transforma em PLI
  offset = _restrictions + _variables;
  for(int i=0; i<_restrictions; i++){
    for(int j=0; j<_variables; j++){
      if(i == j){
        //_matrix[i+1][j+offset] = 1;
        setElement(i+1, j+offset,1);
      }
      else{
        //_matrix[i+1][j+offset] = 0;
        setElement(i+1,j+offset,0);
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
      return SimplexAux();
    }
  }
  return Primal();
}

int Matrix::Primal(){
  int column=0;
  int row=0;
  std::cout << "Primal" << std::endl;
  while(true){
    column = scanC();

    if(column == -1){
      std::cout << "Otima" << std::endl;
      return 1;
    }

    row = scanColumn(column);

    if(row == -1){
      std::cout << "Ilimitada" << std::endl;
      return 0;
    }
    changeBase(row,column);

  }


  return 0;
}

int Matrix::SimplexAux(){
  int column=0;
  int row=0;
  std::cout << "SimplexAux" << std::endl;
  

  return 0;
}



int Matrix::scanC(){
  int offset = _restrictions;

  for(int i=0; i < _variables; i++){
    if(_matrix[0][i+offset] < 0){
      return i+offset;
    }
  }
  return -1;
}


int Matrix::scanColumn(int column){
  int offset = 1;
  float min = 0.0;
  bool isMinValid = false;
  int row;

  for(int i=0; i < _restrictions; i++){
    if(!isMinValid){
      if(_matrix[i+offset][column] > 0){
        min = _matrix[i+offset][_numColumns-1]/_matrix[i+offset][column];
        row = i+offset;
        isMinValid = true;
      }
    }
    else{
      if(_matrix[i+offset][column] > 0){
        if(min > _matrix[i+offset][_numColumns-1]/_matrix[i+offset][column]){
          min = _matrix[i+offset][_numColumns-1]/_matrix[i+offset][column];
          row = i+offset;
        }
      }
    }
  }
  if(isMinValid){
    return row;
  }
  return -1;
}


void Matrix::changeBase(int row, int column){
  for(int i=0; i < _numColumns; i++){
    //_matrix[row][i] =  _matrix[row][i]/_matrix[row][column];
    setElement(row,i,_matrix[row][i]/_matrix[row][column]);
  }

  for(int i=0; i < _numRows; i++){
    if(i != row){
      pivot(row,i,-(_matrix[i][column]));
    }
  }
}

void Matrix::pivot(int row1, int row2, float mult){
  for(int i=0; i < _numColumns; i++){
    //_matrix[row2][i] = _matrix[row2][i] + mult*_matrix[row1][i];
    setElement(row2,i,_matrix[row2][i] + mult*_matrix[row1][i]);
  }

}

void Matrix::printResult(int result){

}
