#include "inc/dsp_processor.h"
#include "esp_dsp.h"
#include <cmath>
#include <algorithm>

float fft_input[N_FFT * 2];
float hann_window[N_FFT];

void init_dsp() {
    dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    dsps_wind_hann_f32(hann_window, N_FFT);
}

void extract_features(TfLiteTensor* input_tensor) {
    bool is_int8 = (input_tensor->type == kTfLiteInt8);
    float scale = input_tensor->params.scale;
    int32_t zero_point = input_tensor->params.zero_point;
    int tensor_idx = 0;

    for (int i = 0; i <= SAMPLE_COUNT - N_FFT; i += HOP_LENGTH) {
        if (tensor_idx >= N_FRAMES * N_MFCC) break;
        for (int k = 0; k < N_FFT; k++) {
            fft_input[k * 2] = audio_float[i + k] * hann_window[k];
            fft_input[k * 2 + 1] = 0;
        }
        dsps_fft2r_fc32(fft_input, N_FFT);
        dsps_bit_rev_fc32(fft_input, N_FFT);

        for (int m = 0; m < N_MFCC; m++) {
            float energy = 0;
            int bin_start = 1 + (m * (m + 1)); 
            int bin_end = bin_start + (m + 2);
            if (bin_end > N_FFT/2) bin_end = N_FFT/2;

            for (int k = bin_start; k < bin_end; k++) {
                float r = fft_input[k * 2];
                float im = fft_input[k * 2 + 1];
                energy += (r*r + im*im);
            }
            float log_energy = log10f(energy + 1e-6);
            float norm_val = (log_energy - (-2.0f)) / 2.0f; 
            if (is_int8) {
                float quant = (norm_val / scale) + zero_point;
                quant = std::max(-128.0f, std::min(127.0f, quant));
                input_tensor->data.int8[tensor_idx++] = (int8_t)quant;
            } else {
                input_tensor->data.f[tensor_idx++] = norm_val;
            }
        }
    }
}