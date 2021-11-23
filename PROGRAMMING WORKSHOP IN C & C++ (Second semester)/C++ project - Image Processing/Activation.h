//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"
#include <cmath>
/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    RELU,
    SOFTMAX
};

// Insert Activation class here...
class Activation
{
 public:
    Activation(ActivationType act_type);
    ActivationType get_activation_type();
    Matrix operator()(Matrix input) const;
 private:
  ActivationType _act_type;
};
#endif //ACTIVATION_H