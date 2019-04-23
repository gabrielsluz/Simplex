#include "PL.hpp"
#include <iostream>

void PL::initialize(int variables, int restrictions){
  _variables = variables;
  _restrictions = restrictions;

  _A.initialize(_restrictions, _variables + _restrictions);
  _b.initialize(_restrictions + 1, 1);
  _c.initialize(1,_variables + _restrictions);
  _Vero.initialize(_restrictions + 1, _restrictions);
}

void PL::inputAandB(){
  float value = 0.0;

  for(int i=0; i < _restrictions; i++){
    for(int j=0; j < _variables; j++){
      std::cin >> value;
      _A.setElement(i,j,value);
    }
    std::cin >> value;
    _b.setElement(i+1,0,value);
  }

  _b.setElement(0,0,0.0);

  //Transforma em PLI
  int offset = _variables;
  for(int i=0; i<_restrictions; i++){
    for(int j=0; j<_variables; j++){
      if(i == j){
        _A.setElement(i, j+offset,1);
      }
      else{
        _A.setElement(i,j+offset,0);
      }
    }
  }
}

void PL::inputC(){
  float value = 0.0;

  for(int i=0; i<_variables; i++){
    std::cin >> value;
    _c.setElement(0,i, -value);
  }

  //Transforma em PLI
  int offset = _variables;
  for(int i=0; i < _restrictions; i++){
    _c.setElement(0,i+offset,0);
  }
}

void PL::createVero(){
  for(int i=0; i<=_restrictions; i++){
    for(int j=0; j<_restrictions; j++){
      if((i-1) == j && i !=0){
        _Vero.setElement(i,j,1);
      }
      else{
        _Vero.setElement(i,j,0);
      }
    }
  }
}

void PL::initAndInput(int variables, int restrictions){
  initialize(variables,restrictions);
  createVero();
  inputC();
  inputAandB();
}

int PL::solve(){
  bool needAux = false;

  for(int i = 0; i < _b._numRows; i++ ){
    if(_b.getElement(i,0) < 0){
      negRow(i);
      needAux = true;
    }
  }
  if(needAux)
    return SimplexAux();

  return Simplex();
}

int PL::Simplex(){
  int column=0;
  int row=0;
  std::cout << "Simplex" << std::endl;
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


int PL::SimplexAux(){


}


void PL::negRow(int row){
  if(row > 0){
    _A.multiplyRow(row-1,-1);
    _b.multiplyRow(row,-1);
    _Vero.multiplyRow(row,-1);
  }
  else if(row == 0){
    _c.multiplyRow(0,-1);
  }
}

int PL::scanC(){

}

int PL::scanColumn(int column){

}

int PL::changeBase(int row, int column){

}
