#include "Matrix.hpp"
#include <iostream>


float** Matrix::allocMatrix(int numRows, int numColumns){
  float** m = new float*[numRows];

  for(int i = 0; i < numRows; i++){
    m[i] = new float[numColumns];
    for(int j = 0; j < numColumns; j++){
      m[i][j] = 0;
    }
  }
  return m;
}

Matrix::Matrix(int restrictions, int variables){
  _numRows = restrictions + 1;
  _numColumns = restrictions + variables + restrictions + 1;
  _restrictions = restrictions;
  _variables = variables;

  _matrix = allocMatrix(_numRows,_numColumns);
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


//Implementacao da parte que lida com a PL auxiliar

int Matrix::SimplexAux(){
  int result=0;
  std::cout << "SimplexAux" << std::endl;

  createAuxMatrix();

  result = PrimalAux();

  if(result == 0)
    std::cout << "Error Auxiliar Ilimitada";

  if(_auxMatrix[0][_numColumns+_restrictions-1] < 0)
    return -2; //Inviavel

  //Copiar parte da auxiliar e substituir a primeira linha
  //Chamar Simplex primal


  freeAuxMatrix();
  return 0;
}

void Matrix::createAuxMatrix(){
  _auxMatrix = allocMatrix(_numRows, _numColumns + _restrictions);
  copyAndChangeMatrix();

}

void Matrix::freeAuxMatrix(){
  for(int i = 0; i < _numRows; i++){
    delete [] _auxMatrix[i];
  }

  delete [] _auxMatrix;
}

void Matrix::setElementAux(int row, int column, float value){
  if(row >= _numRows || column >= _numColumns+_restrictions){
    std::cout << "Error na Aux, dimensoes erradas" << row << "-" << column << std::endl;
    return;
  }
  _auxMatrix[row][column] = value;
}

void Matrix::setFirstRowAux(){
  for(int i=0; i<_restrictions; i++){
    setElementAux(0,i,0);
  }

  int offset = _restrictions;

  for(int i=0; i<_variables; i++){
    setElementAux(0,i+offset,0);
  }

  //Transforma em PLI
  offset = _restrictions + _variables;
  for(int i=0; i<_restrictions; i++){
    setElementAux(0,i+offset,0);
  }
  //Variaveis auxiliares
  offset = _restrictions + _variables + _restrictions;
  for(int i=0; i<_restrictions; i++){
    setElementAux(0,i+offset,1);
  }
  //Ultimo
  setElementAux(0,_restrictions+offset,0);
}

void Matrix::copyAndChangeMatrix(){
  setFirstRowAux();

  for(int i = 1; i < _numRows; i++){
    if(_matrix[i][_numColumns-1] < 0){
      for(int j=0; j < _numColumns -1; j++){
        setElementAux(i, j, -_matrix[i][j]);
      }
      setElementAux(i, _numColumns + _restrictions -1, -_matrix[i][_numColumns-1]);
    }
    else{
      for(int j=0; j < _numColumns -1; j++){
        setElementAux(i, j, _matrix[i][j]);
      }
      setElementAux(i, _numColumns + _restrictions -1, _matrix[i][_numColumns-1]);
    }
  }

  //Variaveis auxliares
  int offset = _restrictions + _variables + _restrictions;
  for(int i=0; i<_restrictions; i++){
    for(int j=0; j<_variables; j++){
      if(i == j){
        setElementAux(i+1, j+offset,1);
      }
      else{
        setElementAux(i+1,j+offset,0);
      }
    }
  }
}


int Matrix::PrimalAux(){
  int column=0;
  int row=0;
  std::cout << "Primal" << std::endl;
  while(true){
    column = scanCAux();

    if(column == -1){
      std::cout << "Otima" << std::endl;
      return 1;
    }

    row = scanColumnAux(column);

    if(row == -1){
      std::cout << "Ilimitada" << std::endl;
      return 0;
    }
    changeBaseAux(row,column);
  }
  return 0;
}


int Matrix::scanCAux(){
  int offset = _restrictions;

  for(int i=0; i < _variables; i++){
    if(_auxMatrix[0][i+offset] < 0){
      return i+offset;
    }
  }
  return -1;
}


int Matrix::scanColumnAux(int column){
  int offset = 1;
  float min = 0.0;
  bool isMinValid = false;
  int row;

  for(int i=0; i < _restrictions; i++){
    if(!isMinValid){
      if(_auxMatrix[i+offset][column] > 0){
        min = _auxMatrix[i+offset][_numColumns + _restrictions-1]/_auxMatrix[i+offset][column];
        row = i+offset;
        isMinValid = true;
      }
    }
    else{
      if(_auxMatrix[i+offset][column] > 0){
        if(min > _auxMatrix[i+offset][_numColumns + _restrictions-1]/_auxMatrix[i+offset][column]){
          min = _auxMatrix[i+offset][_numColumns + _restrictions-1]/_auxMatrix[i+offset][column];
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


void Matrix::changeBaseAux(int row, int column){
  for(int i=0; i < _numColumns + _restrictions; i++){
    //_auxMatrix[row][i] =  _auxMatrix[row][i]/_auxMatrix[row][column];
    setElementAux(row,i,_auxMatrix[row][i]/_auxMatrix[row][column]);
  }

  for(int i=0; i < _numRows; i++){
    if(i != row){
      pivotAux(row,i,-(_auxMatrix[i][column]));
    }
  }
}

void Matrix::pivotAux(int row1, int row2, float mult){
  for(int i=0; i < _numColumns + _restrictions; i++){
    //_auxMatrix[row2][i] = _auxMatrix[row2][i] + mult*_auxMatrix[row1][i];
    setElementAux(row2,i,_auxMatrix[row2][i] + mult*_auxMatrix[row1][i]);
  }
}


void Matrix::printResult(int result){

}
