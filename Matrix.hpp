#ifndef MATRIX_H
#define MATRIX_H


class Matrix{
  private:
    float **_matrix;
    int _numColumns;
    int _numRows;
    int _restrictions;
    int _variables;

    float **_auxMatrix;


  public:

    Matrix(int, int);
    ~Matrix();
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

    void printResult(int);

    void printMatrix();


};




#endif
