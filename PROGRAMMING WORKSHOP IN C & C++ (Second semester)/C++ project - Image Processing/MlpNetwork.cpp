//
// Created by itamar on 08/06/2021.
//

#include "MlpNetwork.h"

MlpNetwork::MlpNetwork(Matrix weights[4], Matrix biases[4])
{
    for (int i =0;i<4;i++){
        _weights[i] = weights[i];
        _biases[i] = biases[i];
    }
}
digit MlpNetwork::operator()(Matrix &input)
{
    // making 4 dense and use them
    Dense d1 = Dense(_weights[0], _biases[0], RELU);
    Dense d2 = Dense(_weights[1], _biases[1], RELU);
    Dense d3 = Dense(_weights[2], _biases[2], RELU);
    Dense d4 = Dense(_weights[3], _biases[3], SOFTMAX);
    Matrix r1 = d1(input);
    Matrix r2 = d2(r1);
    Matrix r3 = d3(r2);
    Matrix r4 = d4(r3);
    float max = 0.0;
    unsigned int dig = 0;
    for (int i=0;i<DIGITS;i++){
        if (r4[i] > max){
            max = r4[i];
            dig = i;
        }
    }
    // make digit
    digit final_digit;
    final_digit.value = dig;
    final_digit.probability = max;
    return final_digit;
}