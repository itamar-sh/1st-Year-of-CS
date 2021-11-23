//
// Created by itamar on 06/06/2021.
//
#include "Matrix.h"
#include <cmath>
#define PRINT_VAL 0.1
Matrix::Matrix ()
{
    cords.cols = 1;
    cords.rows = 1;
    _size = 1;
    _mat = new float[1];
    _mat[0] = 0.0;
}
Matrix::Matrix (int rows, int cols)
{
    cords.cols = cols;
    cords.rows = rows;
    _size = rows * cols;
    _mat = new float[_size];
    for (int i = 0; i < _size; i++)
    {
        _mat[i] = 0.0;
    }
}
Matrix::Matrix (const Matrix &m)
{
    cords.cols = m.cords.cols;
    cords.rows = m.cords.rows;
    _size = m.cords.rows * m.cords.cols;
    _mat = new float[_size];
    for (int i = 0; i < _size; i++)
    {
        _mat[i] = m._mat[i];
    }
}
Matrix::~Matrix ()
{
    delete[] _mat;
}
Matrix &Matrix::transpose ()
{
    float *t_elems = new float[_size];
    for (int i = 0; i < cords.cols; i++)
        {
        for (int j = 0; j < cords.rows; j++)
            {
                t_elems[i * cords.rows + j] = _mat[i + j * cords.cols];
            }
        }
    int t = cords.rows;
    cords.rows = cords.cols;
    cords.cols = t;
    for (int i = 0; i < _size; i++)
        {
            _mat[i] = t_elems[i];
        }
    delete[] t_elems;
    return *this;
}
int Matrix::get_cols () const
{
return cords.cols;
}
int Matrix::get_rows () const
{
return cords.rows;
}
Matrix& Matrix::vectorize ()
{
  cords.cols = 1;
  cords.rows = _size;
  return *this;
}
void Matrix::plain_print ()
{
  for (int i = 0; i < cords.rows; i++)
      {
          for (int j = 0; j < cords.cols; j++)
              {
                  std::cout << _mat[i * cords.cols + j] << " ";
              }
          std::cout << std::endl;
      }
}
Matrix Matrix::dot (Matrix &m)
{
  if (cords.cols != m.cords.cols || cords.rows != m.cords.rows){
      std::cerr << "Error: Matrix not in the right size";
      exit(1);
  }
  float *t_elems = new float[_size];
  for (int i = 0; i < _size; i++)
  {
      t_elems[i] = _mat[i] * m._mat[i];
  }
  Matrix sum_mat (cords.rows, cords.cols);
  for (int i = 0; i < sum_mat._size; i++)
  {
      sum_mat._mat[i] = t_elems[i];
  }
  delete[] t_elems;
  return sum_mat;
}
Matrix Matrix::operator* (const Matrix &m)
{
  if (cords.cols != m.cords.rows){
      std::cerr << "Error: Matrix not in the right size";
      exit(1);
  }
  Matrix sum_mat (cords.rows, m.cords.cols);
  float t_sum = 0.0;
  for (int i = 0; i < cords.rows; i++)
  {
      for (int j = 0; j < m.cords.cols; j++)
          {
              for (int k = 0; k < cords.cols; k++) // sum one cell
                  {
                      float a = (*this)(i, k);
                      float b = m(k, j);
                      t_sum += a * b;
                  }
              sum_mat(i, j) = t_sum;
              t_sum = 0.0;  // reset sun value
          }
  }
  return sum_mat;
}
float Matrix::norm()
{
    float sum =0.0;
    for(int i =0;i<cords.rows;i++){
        for(int j=0;j<cords.cols;j++){
            sum += _mat[i*cords.cols+j]*_mat[i*cords.cols+j];
        }
    }
    sum = sqrtf(sum);
    return sum;
}
Matrix Matrix::operator+ (const Matrix &m)
{
if (cords.cols != m.cords.cols || cords.rows != m.cords.rows){
  return *this; // do nothing and returns the matrix itself
  }
  float *t_elems = new float[_size];
  for (int i = 0; i < _size; i++)
  {
      t_elems[i] = _mat[i] + m._mat[i];
  }
  Matrix sum_mat (cords.rows, cords.cols);
  for (int i = 0; i < sum_mat._size; i++)
  {
      sum_mat._mat[i] = t_elems[i];
  }
  delete[] t_elems;
  return sum_mat;
}
Matrix& Matrix::operator=(const Matrix &m)
{
    if (this == &m){
        return *this;
    }
    this->cords.cols = m.cords.cols;
    this->cords.rows = m.cords.rows;
    this->_size = m._size;
    delete[] this->_mat;
    this->_mat = new float[this->_size];
    for (int i =0;i<m._size;i++){
        this->_mat[i] = m._mat[i];
    }
    return *this;
}
Matrix Matrix::operator*(float n) const
{
    Matrix m = Matrix(cords.rows, cords.cols);
    for(int i = 0; i<_size;i++){
        float val = _mat[i]*n;
        m._mat[i] = val;
    }
    return m;
}
Matrix operator*(float n, const Matrix &m)
{
    return m*n;
}
Matrix& Matrix::operator+=(const Matrix &m)
{
    if (cords.cols != m.cords.cols || cords.rows != m.cords.rows){
      std::cerr << "Error: Matrix not in the right size";
      exit(1);
    }
    for (int i = 0;i<_size;i++){
        _mat[i] += m._mat[i];
    }
    return *this;
}
float& Matrix::operator()(int i, int j)
{
    return _mat[i*cords.cols+j];
}
float& Matrix::operator[](int i)
{
    return _mat[i];
}
float Matrix::operator()(int i, int j) const
{
    return _mat[i*cords.cols+j];
}
float Matrix::operator[](int i) const
{
    return _mat[i];
}
std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
    for(int i =1 ; i<=m.get_rows();i++){
        for(int j =1 ; j<=m.get_cols();j++){
            if(m(i,j)>=PRINT_VAL){
                os << "  ";
            } else {
                os << "**";
            }
        }
        os<< std::endl;
    }
    return os;
}
std::istream& read_binary_file(std::istream& is, Matrix& m)
{
    is.seekg(0, std::istream::end);
    int n_bytes = is.tellg();
    is.seekg(0, std::istream::beg);
    unsigned int n = n_bytes/sizeof(float);
    if (n < (unsigned int)m.get_rows()*m.get_cols()){
        std::cerr << "Error: file not big enough";
        exit(1);
    }
    is.read((char*)m.get_mat(), n_bytes);
    return is;
}