#include <iostream>
#include "Matrix.hpp"


int main(){
  int n, m;  //Numero de restricoes e variaveis

  std::cin >> n >> m;

  Matrix tableaux(m+1,m+n+m+1);

  for(int i=0;i<m;i++){
    for(int j=0;j<m;j++){
      if(i==j && i !=0){
        tableaux.setElement(i,j,1);
      }
      else{
        tableaux.setElement(i,j,0);
      }
    }
  }



  return 0;
}
