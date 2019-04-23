#ifndef SOLVER_H
#define SOLVER_H

#include "array2D.hpp"
#include "PL.hpp"


class Solver{
  private:
    PL pl;
    int _restrictions;
    int _variables;



  public:
/*
    Solver(int, int);
    ~Solver();
    void setElement(int, int, float);
    float** allocMatrix(int, int);

    //Prepara tableau
    void setVeroTM();
    void getVectorC();
    void getAandB();

    //Simplex core
    int Simplex();
    int Primal();
    int SimplexAux();

    //Simplex
    int scanC();
    int scanColumn(int);
    void changeBase(int,int);
    void pivot(int,int,float); // matrix[int2] = matrix[int1]*float + matrix[int2]

    //Simplex Auxiliar Pl
    void createAuxMatrix();
    void freeAuxMatrix();
    void setElementAux(int, int, float);
    void copyAndChangeMatrix();
    void setFirstRowAux();
    int PrimalAux();
    int scanCAux();
    int scanColumnAux(int);
    void changeBaseAux(int,int);
    void pivotAux(int,int,float); // matrix[int2] = matrix[int1]*float + matrix[int2]

    void printResult(int);

    void printMatrix();
    */


};




#endif
