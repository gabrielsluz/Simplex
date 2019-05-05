#ifndef ARRAY2D_H
#define ARRAY2D_H

#define EPS 0.00001

class array2D{
private:
  double **_a;

public:
  int _numRows;
  int _numColumns;

  ~array2D();

  void initialize(int rows, int columns);
  double** allocArray();
  void freeArray();
  void setElement(int row, int column, double value);
  double getElement(int row, int column);
  void printElement(int row, int column);
  void multiplyRow(int row, double mult);
  int scanRow(int row);
  void pivot(int row1, int row2, double mult);
  void printArray();
  //Double op
  double absolute(double x);
  bool isEqual(double a, double b, double eps);
};


#endif
