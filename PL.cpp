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
  for(int i=0; i < _restrictions; i++){
    for(int j=0; j < _restrictions; j++){
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

void PL::createAux(){
  for(int i=0; i < _restrictions; i++){
    for(int j=0; j < _restrictions; j++){
      if(i == j){
        _aux.setElement(i,j,1);
      }
      else{
        _aux.setElement(i,j,0);
      }
    }
  }

  for(int i=0; i < _cAux._numColumns; i++){
      _cAux.setElement(0,i,0);
  }

}

void PL::createViableAux(){
  _aux.initialize(_restrictions,_restrictions);
  _cAux.initialize(1, _c._numColumns + _restrictions);
  createAux();

  float sum = 0;

  //Pivoteamento para criar bases viaveis
  for(int i=0; i < _A._numColumns; i++){
    for(int j=0; j < _A._numRows; j++){
      sum += -_A.getElement(j,i);
    }
    _cAux.setElement(0,i,sum);
    sum = 0;
  }
  for(int i=0; i < _A._numRows; i++){
    _Vero.pivot(i+1,0,-1);
    _b.pivot(i+1,0,-1);
  }
  printAux();
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
    column = _c.scanRow(0);

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
  std::cout << "Simplex Aux" << std::endl;

  createViableAux();


  int column=0;
  int row=0;
  int retValue =0 ;
  while(true){
    column = _cAux.scanRow(0);

    if(column == -1){
      std::cout << "Otima" << std::endl;
      retValue = 1;
      break;
    }

    row = scanColumnAux(column);

    if(row == -1){
      std::cout << "Ilimitada" << std::endl;
      retValue = 0;
      break;
    }
    changeBaseAux(row,column);
  }

  if(retValue == 0 || _b.getElement(0,0) < 0){
    std::cout << "Inviavel" << std::endl;
    return -1;
  }
  else{
    std::cout << "Viavel" << std::endl;
    return 1;
  }



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

//Procura o elemento em uma coluna de A que minimiza a razao _b[i][0]/_A[i][column]
int PL::scanColumn(int column){
  float min = 0.0;
  bool isMinValid = false;
  int row;

  for(int i=0; i < _A._numRows; i++){
    if(!isMinValid){
      if(_A.getElement(i,column) > 0){
        min = _b.getElement(i+1,0)/_A.getElement(i,column);
        row = i;
        isMinValid = true;
      }
    }
    else{
      if(_A.getElement(i,column) > 0){
        if(min > _b.getElement(i+1,0)/_A.getElement(i,column)){
          min = _b.getElement(i+1,0)/_A.getElement(i,column);
          row = i;
        }
      }
    }
  }
  if(isMinValid){
    return row;
  }
  return -1;

}

//Faz o pivoteamento para trocar a base
void PL::changeBase(int row, int column){
  //Faz elemento da nova base ser igual a 1
  float mult = 1.0/_A.getElement(row, column);

  _A.multiplyRow(row,mult);
  _Vero.multiplyRow(row,mult);
  _b.multiplyRow(row+1,mult);

  //Pivoteamento
  //No vetor C
  mult = -_c.getElement(0,column);

  for(int i=0; i < _c._numColumns; i++){
    _c.setElement(0, i, _c.getElement(0,i) + _A.getElement(row,i)*mult);
  }
  _b.setElement(0, 0, _b.getElement(0,0) + _b.getElement(row+1,0)*mult);
  _Vero.pivot(row+1,0,mult);

  for(int i=0; i < _A._numRows; i++){
    if(i != row){
      mult = -_A.getElement(i,column);
      _A.pivot(row,i,mult);
      _Vero.pivot(row+1,i+1,mult);
      _b.pivot(row+1,i+1,mult);
    }
  }
}


//Procura o elemento em uma coluna de A que minimiza a razao _b[i][0]/_A[i][column]
int PL::scanColumnAux(int column){
  float min = 0.0;
  bool isMinValid = false;
  int row;

  if(column < _A._numColumns){
  for(int i=0; i < _A._numRows; i++){
    if(!isMinValid){
      if(_A.getElement(i,column) > 0){
        min = _b.getElement(i+1,0)/_A.getElement(i,column);
        row = i;
        isMinValid = true;
      }
    }
    else{
      if(_A.getElement(i,column) > 0){
        if(min > _b.getElement(i+1,0)/_A.getElement(i,column)){
          min = _b.getElement(i+1,0)/_A.getElement(i,column);
          row = i;
        }
      }
    }
  }
  if(isMinValid){
    return row;
  }
  return -1;
}
  else{
    column -= _A._numColumns;
    for(int i=0; i < _aux._numRows; i++){
      if(!isMinValid){
        if(_aux.getElement(i,column) > 0){
          min = _b.getElement(i+1,0)/_aux.getElement(i,column);
          row = i;
          isMinValid = true;
        }
      }
      else{
        if(_aux.getElement(i,column) > 0){
          if(min > _b.getElement(i+1,0)/_aux.getElement(i,column)){
            min = _b.getElement(i+1,0)/_aux.getElement(i,column);
            row = i;
          }
        }
      }
    }
    if(isMinValid){
      return row;
    }
    return -1;
  }

}

//Faz o pivoteamento para trocar a base
void PL::changeBaseAux(int row, int column){
  if(column < _A._numColumns){
    //Faz elemento da nova base ser igual a 1
    float mult = 1.0/_A.getElement(row, column);
    int offset = 0;

    _A.multiplyRow(row,mult);
    _aux.multiplyRow(row,mult);
    _Vero.multiplyRow(row,mult);
    _b.multiplyRow(row+1,mult);

    //Pivoteamento
    //No vetor C
    mult = -_cAux.getElement(0,column);

    for(int i=0; i < _A._numColumns; i++){
      _cAux.setElement(0, i, _cAux.getElement(0,i) + _A.getElement(row,i)*mult);
    }
    offset = _A._numColumns;
    for(int i = offset; i < _cAux._numColumns; i++){
      _cAux.setElement(0, i, _cAux.getElement(0,i) + _aux.getElement(row,i - offset)*mult);
    }

    _b.setElement(0, 0, _b.getElement(0,0) + _b.getElement(row+1,0)*mult);
    _Vero.pivot(row+1,0,mult);

    for(int i=0; i < _A._numRows; i++){
      if(i != row){
        mult = -_A.getElement(i,column);
        std::cout << "Mult = " << mult << std::endl;
        _A.pivot(row,i,mult);
        _aux.pivot(row,i,mult);
        _Vero.pivot(row+1,i+1,mult);
        _b.pivot(row+1,i+1,mult);
      }
    }
  }
  else{
    column -= _A._numColumns;
    //Faz elemento da nova base ser igual a 1
    float mult = 1.0/_aux.getElement(row, column);
    int offset = 0;

    _A.multiplyRow(row,mult);
    _aux.multiplyRow(row,mult);
    _Vero.multiplyRow(row,mult);
    _b.multiplyRow(row+1,mult);

    //Pivoteamento
    //No vetor C
    mult = -_cAux.getElement(0,column);

    for(int i=0; i < _A._numColumns; i++){
      _cAux.setElement(0, i, _cAux.getElement(0,i) + _A.getElement(row,i)*mult);
    }
    offset = _A._numColumns;
    for(int i = offset; i < _cAux._numColumns; i++){
      _cAux.setElement(0, i, _cAux.getElement(0,i) + _aux.getElement(row,i - offset)*mult);
    }

    _b.setElement(0, 0, _b.getElement(0,0) + _b.getElement(row+1,0)*mult);
    _Vero.pivot(row+1,0,mult);

    for(int i=0; i < _A._numRows; i++){
      if(i != row){
        mult = -_aux.getElement(i,column);
        std::cout << "Mult = " << mult << std::endl;
        _A.pivot(row,i,mult);
        _aux.pivot(row,i,mult);
        _Vero.pivot(row+1,i+1,mult);
        _b.pivot(row+1,i+1,mult);
      }
    }
  }
}


float PL::getOtimo(){
  return _b.getElement(0,0);
}

void PL::printPl(){
  std::cout << "Vero" << std::endl;
  _Vero.printArray();
  std::cout << "C" << std::endl;
  _c.printArray();
  std::cout << "A" << std::endl;
  _A.printArray();
  std::cout << "b" << std::endl;
  _b.printArray();
}

void PL::printAux(){
  std::cout << "Vero" << std::endl;
  _Vero.printArray();
  std::cout << "CAux" << std::endl;
  _cAux.printArray();
  std::cout << "A" << std::endl;
  _A.printArray();
  std::cout << "Aux" << std::endl;
  _aux.printArray();
  std::cout << "b" << std::endl;
  _b.printArray();
}

void PL::printVero(){
  _Vero.printArray();
}
