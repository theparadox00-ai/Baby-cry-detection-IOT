#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H
#include "config.h"
#include <tensorflow/lite/c/common.h>

bool setup_models();
float run_binary_inference();
void run_multi_inference(int* predicted_index, float* confidence);

TfLiteTensor* get_binary_input();
TfLiteTensor* get_multi_input();

#endif