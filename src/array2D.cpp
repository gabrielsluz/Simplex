#include "array2D.hpp"
#include <iostream>


void array2D::initialize(int rows, int columns){
  _numRows = rows;
  _numColumns = columns;

  _a = allocArray();
}

double** array2D::allocArray(){
  double** m = new double*[_numRows];

  for(int i = 0; i < _numRows; i++){
    m[i] = new double[_numColumns];
    for(int j = 0; j < _numColumns; j++){
      m[i][j] = 0;
    }
  }
  return m;
}

array2D::~array2D(){
  freeArray();
}

void array2D::freeArray(){
  for(int i = 0; i < _numRows; i++){
    delete [] _a[i];
  }

  delete [] _a;
}

void array2D::setElement(int row, int column, double value){
  /*if(row >= _numRows || column >= _numColumns){
    std::cout << "Error, dimensoes erradas" << row << "-" << column << std::endl;
    return;
  }*/
  _a[row][column] = value;
}

double array2D::getElement(int row, int column){
  /*if(row >= _numRows || column >= _numColumns){
    std::cout << "Error, dimensoes erradas" << row << "-" << column << std::endl;
    return 0;
  }*/
  return _a[row][column];
}

void array2D::printElement(int row, int column){
  if(row >= _numRows || column >= _numColumns){
    std::cout << "Error, dimensoes erradas" << row << "-" << column << std::endl;
    return;
  }
  std::cout << _a[row][column];
}

//Multiplica uma linha da matriz por um numero
void array2D::multiplyRow(int row, double mult){
  for(int i=0; i < _numColumns; i++){
    _a[row][i] = mult*_a[row][i];
  }
}

//Varre linha da matriz verificando se possui um elemento negativo e retorna o primeiro que encontrar
int array2D::scanRow(int row){
  for(int i=0; i < _numColumns; i++){
    if(!isEqual(_a[row][i],0,EPS) && _a[row][i] < 0){
      return i;
    }
  }
  return -1;
}

//Soma a linha row1*mult com row2, colocando o resultado na linha row2
void array2D::pivot(int row1, int row2, double mult){
  for(int i=0; i < _numColumns; i++){
    setElement(row2, i, mult*getElement(row1,i) + getElement(row2,i));
  }
}

void array2D::printArray(){
  for(int i = 0; i < _numRows; i++){
    for(int j=0; j < _numColumns; j++){
      std::cout << _a[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

double array2D::absolute(double x){
  return x < 0 ? -x : x;
}

bool array2D::isEqual(double a, double b, double eps){
  double diff = absolute(a - b);
  a = absolute(a);
  b = absolute(b);

  double largest = b > a ? b : a;
  largest = largest > 1 ? largest : 1;

  if(diff <= largest*eps)
    return true;
  return false;
}
