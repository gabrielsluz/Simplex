#ifndef MATRIX_H
#define MATRIX_H


class Matrix{
  private:
    float** _matrix;
    int _restrictions;
    int _variables;
    int _numRows;
    int _numColumns;

    float **_auxMatrix;


  public:

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
    void pivotAux(int row1,int row2,float mult); // matrix[int2] = matrix[int1]*float + matrix[int2]

    void printResult(int);

    void printMatrix();


};




#endif
