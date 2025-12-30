#ifndef DSP_PROCESSOR_H
#define DSP_PROCESSOR_H
#include "config.h"
#include <tensorflow/lite/c/common.h> 

void init_dsp();
void extract_features(TfLiteTensor* input_tensor);

#endif