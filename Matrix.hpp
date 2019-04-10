#ifndef MATRIX_H
#define MATRIX_H


class Matrix{
  private:
    float **_matrix;
    int _numColumns;
    int _numRows;


  public:
    Matrix(int, int);
    ~Matrix();
    void setElement(int,int,float);
    void printMatrix();

};




#endif
