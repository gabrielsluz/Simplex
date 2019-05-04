#ifndef ARRAY2D_H
#define ARRAY2D_H


class array2D{
private:
  float **_a;

public:
  int _numRows;
  int _numColumns;

  ~array2D();

  void initialize(int rows, int columns);
  float** allocArray();
  void freeArray();
  void setElement(int row, int column, float value);
  float getElement(int row, int column);
  void printElement(int row, int column);
  void multiplyRow(int row, float mult);
  int scanRow(int row);
  void pivot(int row1, int row2, float mult);
  void printArray();
};


#endif
