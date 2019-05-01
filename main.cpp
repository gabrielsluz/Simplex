#include <iostream>
#include "PL.hpp"


int main(){

  int n, m;  //Numero de restricoes e variaveis
  int result;

  std::cin >> n >> m;

  PL *pl = new PL;

  pl->initAndInput(n,m);

  result = pl->solve();
  std::cout << pl->getOtimo() << std::endl;
  pl->printVero();


  delete pl;


  return 0;
}
