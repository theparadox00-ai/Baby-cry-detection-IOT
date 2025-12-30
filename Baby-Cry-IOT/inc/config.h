#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define I2S_WS      11
#define I2S_SD      10
#define I2S_SCK     12
#define I2S_PORT    I2S_NUM_0

#define SAMPLE_RATE     16000
#define SAMPLE_COUNT    24000  
#define CONFIDENCE_THRESHOLD 0.70

#define N_MFCC          13
#define N_FFT           2048
#define HOP_LENGTH      512
#define N_FRAMES        47    

extern int16_t audio_buffer[SAMPLE_COUNT];
extern float   audio_float[SAMPLE_COUNT];
extern const char* CRY_LABELS[];

#endif