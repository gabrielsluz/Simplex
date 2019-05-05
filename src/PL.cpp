#include "PL.hpp"
#include <iostream>
#include <iomanip>

void PL::initialize(int variables, int restrictions){
  _variables = variables;
  _restrictions = restrictions;

  _A.initialize(_restrictions, _variables + _restrictions);
  _b.initialize(_restrictions + 1, 1);
  _c.initialize(1,_variables + _restrictions);
  _base = new int[_restrictions];
}

void PL::inputAandB(){
  double value = 0.0;

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
  double value = 0.0;

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

  double sum = 0;

  //Pivoteamento para criar bases viaveis
  for(int i=0; i < _A._numColumns; i++){
    for(int j=0; j < _A._numRows; j++){
      sum += -_A.getElement(j,i);
    }
    _cAux.setElement(0,i,sum);
    sum = 0;
  }
  for(int i=0; i < _A._numRows; i++){
    _b.pivot(i+1,0,-1);
  }
}

void PL::initAndInput(int variables, int restrictions){
  initialize(variables,restrictions);
  inputC();
  inputAandB();
}

//Chama o Simplex ou SimplexAux e imprime o output
void PL::solve(){
  bool needAux = false;
  int retValue = 0;

  for(int i = 0; i < _b._numRows; i++ ){
    if(_b.getElement(i,0) < 0){
      negRow(i);
      needAux = true;
    }
  }
  if(needAux){
    retValue = SimplexAux();
  }
  else{
    retValue =  Simplex();
  }

  if(retValue == 1){
    //Otima  std::cout << std::fixed << std::setprecision(7);
    std::cout << std::fixed << std::setprecision(7);
    std::cout << "otima" << std::endl;
    std::cout << _b.getElement(0,0) << std::endl;
    printOptSol();
    for(int i= _variables; i < _variables + _restrictions; i++){
      std::cout << _c.getElement(0,i) << " ";
    }
    std::cout << std::endl;
  }
  else if(retValue == 0){
    //Ilimitada
    std::cout << std::fixed << std::setprecision(7);
    std::cout << "ilimitada" << std::endl;
    printOptSol();
    printUnlCert();
    std::cout << std::endl;
  }
  else if(retValue == -1){
    //Inviavel
    std::cout << std::fixed << std::setprecision(7);
    std::cout << "inviavel" << std::endl;
    for(int i= _variables; i < _variables + _restrictions; i++){
      std::cout << _cAux.getElement(0,i) << " ";
    }
    std::cout << std::endl;
  }
}

int PL::Simplex(){
  int column=0;
  int row=0;
 //std::cout << "Simplex" << std::endl;

 for(int i=0; i < _restrictions; i++){
   _base[i] = _variables + i;
 }

  while(true){
    //printPl();
    column = _c.scanRow(0);

    if(column == -1){
      //std::cout << "Otima" << std::endl;
      return 1;
    }

    row = scanColumn(column);

    if(row == -1){
      _UnlimitedColumn = column;
      //std::cout << "Ilimitada" << std::endl;
      return 0;
    }
    changeBase(row,column);
    _base[row] = column;
  }
  return 0;
}


int PL::SimplexAux(){
  //std::cout << "Simplex Aux" << std::endl;

  createViableAux();
  for(int i=0; i < _restrictions; i++){
    _base[i] = _variables + _restrictions + i;
  }


  int column=0;
  int row=0;
  int retValue =0;
  double num = 0.0;
  double mult = 0;

  while(true){
    //printAux();
    column = _cAux.scanRow(0);

    if(column == -1){
      //std::cout << "Otima" << std::endl;
      retValue = 1;
      break;
    }

    row = scanColumnAux(column);

    if(row == -1){
      //std::cout << "Ilimitada" << std::endl;
      retValue = 0;
      break;
    }
    changeBaseAux(row,column);
    _base[row] = column;
  }

  if(retValue == 0 || (!isEqual(_b.getElement(0,0),0,EPS) && _b.getElement(0,0) < 0)){
  //if(retValue == 0 || !isEqual(_b.getElement(0,0),0,EPS)){
    //std::cout << "Inviavel" << std::endl;
    return -1;
  }

  //std::cout << "Viavel" << std::endl;
  //Pivoteamento para criar bases canonicas
//  std::cout << "Rumo ao Simplex" << std::endl;
//  printPl();
  for(int i=0; i < _A._numRows; i++){
    //std::cout << _base[i] << std::endl;
    if(_base[i] < _variables+_restrictions){
      mult = -_c.getElement(0,_base[i]);
    }
    else{
      mult = 0;
    }

    for(int j=0; j < _A._numColumns; j++){
      num = _c.getElement(0,j) + (mult* _A.getElement(i,j));
      _c.setElement(0,j,num);
    }
    _b.pivot(i+1,0,mult);
  }
//  std::cout << "Preparada" << std::endl;
//  printPl();

  return Simplex();
}


void PL::negRow(int row){
  if(row > 0){
    _A.multiplyRow(row-1,-1);
    _b.multiplyRow(row,-1);
  }
  else if(row == 0){
    _c.multiplyRow(0,-1);
  }
}

//Procura o elemento em uma coluna de A que minimiza a razao _b[i][0]/_A[i][column]
int PL::scanColumn(int column){
  double min = 0.0;
  bool isMinValid = false;
  int row;

  for(int i=0; i < _A._numRows; i++){
    if(!isMinValid){
      if(!isEqual(_A.getElement(i,column),0,EPS) && _A.getElement(i,column) > 0){
        min = _b.getElement(i+1,0)/_A.getElement(i,column);
        row = i;
        isMinValid = true;
      }
    }
    else{
      if(!isEqual(_A.getElement(i,column),0,EPS) && _A.getElement(i,column) > 0){
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
  double mult = 1.0/_A.getElement(row, column);

  _A.multiplyRow(row,mult);
  _b.multiplyRow(row+1,mult);

  //Pivoteamento
  //No vetor C
  mult = -_c.getElement(0,column);

  for(int i=0; i < _c._numColumns; i++){
    _c.setElement(0, i, _c.getElement(0,i) + _A.getElement(row,i)*mult);
  }
  _b.setElement(0, 0, _b.getElement(0,0) + _b.getElement(row+1,0)*mult);

  for(int i=0; i < _A._numRows; i++){
    if(i != row){
      mult = -_A.getElement(i,column);
      _A.pivot(row,i,mult);
      _b.pivot(row+1,i+1,mult);
    }
  }
}


//Procura o elemento em uma coluna de A que minimiza a razao _b[i][0]/_A[i][column]
int PL::scanColumnAux(int column){
  double min = 0.0;
  bool isMinValid = false;
  int row;

  if(column < _A._numColumns){
  for(int i=0; i < _A._numRows; i++){
    if(!isMinValid){
      if(!isEqual(_A.getElement(i,column),0,EPS) && _A.getElement(i,column) > 0){
        min = _b.getElement(i+1,0)/_A.getElement(i,column);
        row = i;
        isMinValid = true;
      }
    }
    else{
      if(!isEqual(_A.getElement(i,column),0,EPS) && _A.getElement(i,column) > 0){
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
        if(!isEqual(_aux.getElement(i,column),0,EPS) && _aux.getElement(i,column) > 0){
          min = _b.getElement(i+1,0)/_aux.getElement(i,column);
          row = i;
          isMinValid = true;
        }
      }
      else{
        if(!isEqual(_aux.getElement(i,column),0,EPS) && _aux.getElement(i,column) > 0){
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
    double mult = 1.0/_A.getElement(row, column);
    int offset = 0;

    _A.multiplyRow(row,mult);
    _aux.multiplyRow(row,mult);
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

    for(int i=0; i < _A._numRows; i++){
      if(i != row){
        mult = -_A.getElement(i,column);
        _A.pivot(row,i,mult);
        _aux.pivot(row,i,mult);
        _b.pivot(row+1,i+1,mult);
      }
    }
  }
  else{
    column -= _A._numColumns;
    //Faz elemento da nova base ser igual a 1
    double mult = 1.0/_aux.getElement(row, column);
    int offset = 0;

    _A.multiplyRow(row,mult);
    _aux.multiplyRow(row,mult);
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

    for(int i=0; i < _A._numRows; i++){
      if(i != row){
        mult = -_aux.getElement(i,column);
        _A.pivot(row,i,mult);
        _aux.pivot(row,i,mult);
        _b.pivot(row+1,i+1,mult);
      }
    }
  }
}


double PL::getOtimo(){
  return _b.getElement(0,0);
}

void PL::printPl(){
  std::cout << "C" << std::endl;
  _c.printArray();
  std::cout << "A" << std::endl;
  _A.printArray();
  std::cout << "b" << std::endl;
  _b.printArray();
}

void PL::printAux(){
  std::cout << "CAux" << std::endl;
  _cAux.printArray();
  std::cout << "A" << std::endl;
  _A.printArray();
  std::cout << "Aux" << std::endl;
  _aux.printArray();
  std::cout << "b" << std::endl;
  _b.printArray();
}


void PL::printOptSol(){
  double *sol = new double[_variables];

  for(int i=0; i < _variables; i++){
    sol[i] = 0;
  }

  int pos = -1;
  bool foundOne = false;
  for(int i=0; i < _variables; i++){
    foundOne = false;
    pos = -1;
    if(!isEqual(_c.getElement(0,i),0,EPS)){
      continue;
    }
    for(int j=0; j < _A._numRows; j++){
      if(!isEqual(_A.getElement(j,i),0,EPS) && !isEqual(_A.getElement(j,i),1,EPS)){
        pos = -1;
        break;
      }
      else if(isEqual(_A.getElement(j,i),1,EPS)){
        if(!foundOne){
          foundOne = true;
          pos = j;
        }
        else{
          pos = -1;
        }
      }
    }
    if(pos != -1){
      sol[i] = _b.getElement(pos+1,0);
    }
  }

  for(int i=0; i < _variables; i++){
    std::cout << sol[i] <<  " ";
  }
  std::cout << std::endl;

  delete [] sol;

}

void PL::printUnlCert(){
  double *certify = new double[_variables + _restrictions];
  int pos = -1;
  bool foundOne = false;

  certify[_UnlimitedColumn] = 1;

  for(int i=0; i < _variables; i++){
    foundOne = false;
    pos = -1;
    if(!isEqual(_c.getElement(0,i),0,EPS)){
      if(i != _UnlimitedColumn){
        certify[i] = 0;
      }
      continue;
    }
    for(int j=0; j < _A._numRows; j++){
      if(!isEqual(_A.getElement(j,i),0,EPS) && !isEqual(_A.getElement(j,i),1,EPS)){
        pos = -1;
        break;
      }
      else if(isEqual(_A.getElement(j,i),1,EPS)){
        if(!foundOne){
          foundOne = true;
          pos = j;
        }
        else{
          pos = -1;
        }
      }
    }
    if(pos != -1){
      certify[i] = -_A.getElement(pos,_UnlimitedColumn);
    }
    else{
      certify[i] = 0;
    }
  }

  for(int i=0; i < _variables; i++){
    std::cout << certify[i] << " ";
  }


  delete [] certify;
}

double PL::absolute(double x){
  return x < 0 ? -x : x;
}

bool PL::isEqual(double a, double b, double eps){
  double diff = absolute(a - b);
  a = absolute(a);
  b = absolute(b);

  double largest = b > a ? b : a;
  largest = largest > 1 ? largest : 1;

  if(diff <= largest*eps)
    return true;
  return false;
}
