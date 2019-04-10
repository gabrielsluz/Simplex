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
    Matrix(int, int, int, int);
    ~Matrix();
    void setElement(int, int, float);

    void setVeroTM();
    void getVectorC();
    void getAandB();

    int Simplex();
    int Primal();
    int Dual();

    void printResult(int);


    void printMatrix();


};




#endif
