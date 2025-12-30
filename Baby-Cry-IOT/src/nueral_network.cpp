#include "inc/nueral_network.h"
#include <TensorFlowLite_ESP32.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>

#include "model/binary_model.h"
#include "model/multi_model.h"

tflite::MicroErrorReporter micro_error_reporter;
tflite::AllOpsResolver resolver;

const tflite::Model* model_bin = nullptr;
const tflite::Model* model_multi = nullptr;
tflite::MicroInterpreter* interp_bin = nullptr;
tflite::MicroInterpreter* interp_multi = nullptr;

const int kArenaSize = 60000;
uint8_t bin_arena[kArenaSize];
uint8_t multi_arena[kArenaSize];

bool setup_models() {
    model_bin = tflite::GetModel(binary_model);
    model_multi = tflite::GetModel(multi_model);
    
    static tflite::MicroInterpreter static_bin(model_bin, resolver, bin_arena, kArenaSize, &micro_error_reporter);
    interp_bin = &static_bin;
    
    static tflite::MicroInterpreter static_multi(model_multi, resolver, multi_arena, kArenaSize, &micro_error_reporter);
    interp_multi = &static_multi;

    if (interp_bin->AllocateTensors() != kTfLiteOk) return false;
    if (interp_multi->AllocateTensors() != kTfLiteOk) return false;
    
    return true;
}

TfLiteTensor* get_binary_input() { return interp_bin->input(0); }
TfLiteTensor* get_multi_input() { return interp_multi->input(0); }

float run_binary_inference() {
    if (interp_bin->Invoke() != kTfLiteOk) return 0.0f;
    return 1.0f - interp_bin->output(0)->data.f[0];
}

void run_multi_inference(int* predicted_index, float* confidence) {
    if (interp_multi->Invoke() != kTfLiteOk) return;
    
    TfLiteTensor* out = interp_multi->output(0);
    float max_score = 0;
    int max_idx = 0;

    for(int i=0; i<5; i++) {
        float val;
        if(out->type == kTfLiteInt8) {
            val = (out->data.int8[i] - out->params.zero_point) * out->params.scale;
        } else {
            val = out->data.f[i];
        }
        if(val > max_score) { max_score = val; max_idx = i; }
    }
    *predicted_index = max_idx;
    *confidence = max_score;
}