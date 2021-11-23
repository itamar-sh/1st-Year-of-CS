//
// Created by itamar on 08/06/2021.
//

#include "Activation.h"

Activation::Activation(ActivationType act_type)
{
    _act_type = act_type;
}

ActivationType Activation::get_activation_type()
{
    return _act_type;
}
Matrix Activation::operator()(Matrix input) const
{
    Matrix new_mat = input;
    if(_act_type == RELU){
        for (int i=0;i<new_mat.get_rows()*new_mat.get_cols();i++){
            if (input[i] <0){
                input[i] = 0;
            }
        }
    } else {
        if(_act_type == SOFTMAX){
            float denominator = 0.0;
            for (int i=0;i<new_mat.get_rows()*new_mat.get_cols();i++){
                denominator += (float)std::exp((double)input[i]);
            }
            for (int i=0;i<new_mat.get_rows()*new_mat.get_cols();i++){
                input[i] = (float)std::exp((double)input[i])/denominator;
            }
        }
    }
    return input;
}
