#ifndef MATRIX_H
#define MATRIX_H


class Matrix{
  private:
    float **_matrix;
    int _numColumns;
    int _numRows;
    int _restrictions;
    int _variables;


  public:

    Matrix(int, int);
    ~Matrix();
    void setElement(int, int, float);

    //Prepara tableau
    void setVeroTM();
    void getVectorC();
    void getAandB();

    //Simplex core
    int Simplex();
    int Primal();
    int Dual();

    //Simplex aux
    int scanC();
    int scanColumn(int);
    void changeBase(int,int);
    void pivot();

    void printResult(int);

    void printMatrix();


};




#endif
