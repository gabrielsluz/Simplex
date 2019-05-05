#ifndef PL_H
#define PL_H

#include "array2D.hpp"



class PL{
  array2D _A;
  array2D _b;
  array2D _c;
  array2D _aux;
  array2D _cAux;

  int _restrictions;
  int _variables;

  int _UnlimitedColumn;
  int *_base;

//Input e inicializar
  void initialize(int variables, int restrictions);
  void inputAandB();
  void inputC();
  void createAux();

//Simplex
  int Simplex();
  int SimplexAux();

//Funcoes auxiliares no Simplex
  void negRow(int row);
  int scanColumn(int column);
  void changeBase(int row,int column);

//Funcoes auxiliares no SimplexAux
  int scanColumnAux(int column);
  void changeBaseAux(int row,int column);
  void createViableAux();

//Output
  void printOptSol();
  void printUnlCert();

//Double op
  double absolute(double x);
  bool isEqual(double a, double b, double eps);

public:
  void initAndInput(int variables, int restrictions);
  void solve();
  double getOtimo();
  void printPl();
  void printAux();

};

#endif
