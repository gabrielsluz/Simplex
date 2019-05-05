#include <iostream>
#include "PL.hpp"


int main(){

  int n, m;  //Numero de restricoes e variaveis
  int result;

  std::cin >> n >> m;

  PL *pl = new PL;

  pl->initAndInput(m,n);
  //pl->printPl();

  //pl->printPl();

  pl->solve();

  //pl->printPl();

  delete pl;


  return 0;
}
