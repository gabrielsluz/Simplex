#ifndef PL_H
#define PL_H

#include "array2D.hpp"


class PL{
  array2D _A;
  array2D _b;
  array2D _c;
  array2D _Vero;
  array2D _aux;
  array2D _cAux;

  int _restrictions;
  int _variables;

//Input e inicializar
  void initialize(int variables, int restrictions);
  void inputAandB();
  void inputC();
  void createVero();

//Simplex
  int Simplex();
  int SimplexAux();

//Funcoes auxiliares no Simplex
  void negRow(int row);
  int scanColumn(int column);
  void changeBase(int row,int column);

public:
  void initAndInput(int variables, int restrictions);
  int solve();
  float getOtimo();
  void printPl();
  void printVero();

};

#endif
