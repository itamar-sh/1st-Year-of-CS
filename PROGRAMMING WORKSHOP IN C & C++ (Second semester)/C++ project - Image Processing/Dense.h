#ifndef C___PROJECT_DENSE_H
#define C___PROJECT_DENSE_H

#include "Activation.h"


// implement class Dense here...
class Dense
{
 public:
  Dense(Matrix &w, Matrix &bias, ActivationType act);
  Matrix get_weights() const;
  Matrix get_bias() const;
  Activation get_activation();
  Matrix operator()(Matrix &input);
 private:
  Matrix _w;
  Matrix _bias;
  ActivationType _act;
};

#endif //C___PROJECT_DENSE_H