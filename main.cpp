#include <iostream>
#include "Matrix.hpp"


int main(){

  int n, m;  //Numero de restricoes e variaveis
  int result;

  std::cin >> n >> m;

  Matrix tableau(n,m);


  tableau.setVeroTM();
  tableau.getVectorC();
  tableau.getAandB();


  std::cout << std::endl;
  tableau.printMatrix();

  result = tableau.Simplex();
  tableau.printResult(result);



  return 0;
}
