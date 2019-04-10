#include <iostream>
#include "Matrix.hpp"


int main(){
  int n, m;  //Numero de restricoes e variaveis

  std::cin >> n >> m;

  Matrix tableaux(m+1,m+n+m+1,n,m);


  tableaux.setVeroTM();
  tableaux.getVectorC();
  tableaux.getAandB();


  std::cout << std::endl;
  tableaux.printMatrix();



  return 0;
}
