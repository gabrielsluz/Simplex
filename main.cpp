#include <iostream>
#include "Matrix.hpp"


int main(){
  int n, m;  //Numero de restricoes e variaveis
  int result;

  std::cin >> n >> m;

  Matrix tableaux(m+1,m+n+m+1,n,m);


  tableaux.setVeroTM();
  tableaux.getVectorC();
  tableaux.getAandB();


  std::cout << std::endl;
  tableaux.printMatrix();

  result = tableaux.Simplex();
  tableaux.printResult(result);



  return 0;
}
