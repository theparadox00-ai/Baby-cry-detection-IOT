#include <Arduino.h>
#include "inc/config.h"
#include "inc/audio_provider.h"
#include "inc/dsp_processor.h"
#include "inc/neural_network.h"

int16_t audio_buffer[SAMPLE_COUNT];
float   audio_float[SAMPLE_COUNT];
const char* CRY_LABELS[] = {"Hungry", "Tired", "Belly Pain", "Discomfort", "Burping"};

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println(" Initializing Baby-Cry Monitor...");

    setup_i2s();
    init_dsp();
    
    if (!setup_models()) {
        Serial.println(" Model Allocation Failed!");
        while(1); 
    }

    Serial.println("System Initialization Complete .");
}

void loop() {
    record_audio();
    TfLiteTensor* bin_input = get_binary_input();
    extract_features(bin_input); 
    
    float cry_conf = run_binary_inference();
    Serial.printf("Cry Conf: %.1f%%\n", cry_conf * 100);

    if (cry_conf > CONFIDENCE_THRESHOLD) {
        Serial.println("Cry Detected! Analyzing cause...");
        
        TfLiteTensor* multi_input = get_multi_input();
        extract_features(multi_input); 
        
        int reason_idx = 0;
        float reason_conf = 0.0f;
        run_multi_inference(&reason_idx, &reason_conf);

        Serial.printf("DIAGNOSIS: %s (%.1f%%)\n", CRY_LABELS[reason_idx], reason_conf * 100);
        
        delay(3000); 
    }
}