//
// Created by itamar on 08/06/2021.
//

#include "Dense.h"

Dense::Dense(Matrix &w, Matrix &bias, ActivationType act)
{
  _w = Matrix(w);
  _bias = Matrix(bias);
  _act = act;
}

Matrix Dense::get_weights() const
{
  return _w;
}

Matrix Dense::get_bias() const
{
  return _bias;
}
Activation Dense::get_activation()
{
  Activation f(_act);
  return f;
}
Matrix Dense::operator()(Matrix &input)
{
  Matrix output = _w*(input);
  output += _bias;
  Activation f(_act);  // make the actual func
  Matrix final = f(output);
  return final;
}