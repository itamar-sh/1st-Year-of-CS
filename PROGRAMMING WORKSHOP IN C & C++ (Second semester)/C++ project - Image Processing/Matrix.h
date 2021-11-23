// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims {
    int rows, cols;
} matrix_dims;

// Insert Matrix class here...
class Matrix {
 public:
  Matrix ();
  Matrix (int rows, int cols);
  Matrix (const Matrix &m);// : Matrix(m._rows, m._cols){}
  ~Matrix ();
  int get_cols () const;
  int get_rows () const;
  // getter need to erase
  float *get_mat ()
  {
      return _mat;
  }
  Matrix &transpose ();
  Matrix &vectorize ();
  void plain_print ();
  Matrix dot (Matrix &m);
  float norm();
  Matrix operator* (const Matrix &m);
  Matrix operator+ (const Matrix &m);
  Matrix& operator=(const Matrix &m);
  friend Matrix operator*(float n, const Matrix &m);
  Matrix operator*(float n) const;
  Matrix& operator+=(const Matrix& vec);
  float& operator()(int i, int j);
  float& operator[](int i);
  float operator()(int i, int j) const;
  float operator[](int i) const;
  friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
  friend std::istream& read_binary_file(std::istream& is, Matrix& m);
  // setter - need to erase!
  void set_at (int index, int value)
  {
      _mat[index] = (float) value;
  }
 private:
  matrix_dims cords{};
  float *_mat;
  int _size;
};

#endif //MATRIX_H
