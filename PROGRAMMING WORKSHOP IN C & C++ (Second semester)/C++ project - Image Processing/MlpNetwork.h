//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Activation.h"
#include "Dense.h"
#include "Digit.h"

#define MLP_SIZE 4
#define DIGITS 10

//
const matrix_dims img_dims = {28, 28};

const matrix_dims weights_dims[] = {{128, 784},
                                    {64, 128},
                                    {20, 64},
                                    {10, 20}};

const matrix_dims bias_dims[]    = {{128, 1},
                                    {64, 1},
                                    {20, 1},
                                    {10, 1}};

// Insert MlpNetwork class here...
class MlpNetwork
{
 public:
    MlpNetwork(Matrix weights[4], Matrix biases[4]);
    digit operator()(Matrix &input);
 private:
    Matrix _weights[4];
    Matrix _biases[4];
};

#endif // MLPNETWORK_H
