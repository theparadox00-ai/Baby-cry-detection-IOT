#include "inc/audio_provider.h"
#include "driver/i2s.h"

int32_t i2s_raw_buffer[SAMPLE_COUNT];

void setup_i2s() {
    const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64
    };
    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = -1,
        .data_in_num = I2S_SD
    };
    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_PORT, &pin_config);
}

void record_audio() {
    size_t bytes_read;
    i2s_read(I2S_PORT, i2s_raw_buffer, sizeof(i2s_raw_buffer), &bytes_read, portMAX_DELAY);
    
    int samples_read = bytes_read / 4;
    for (int i = 0; i < samples_read && i < SAMPLE_COUNT; i++) {
        int16_t sample = (int16_t)(i2s_raw_buffer[i] >> 16);
        audio_buffer[i] = sample;
        audio_float[i] = (float)sample / 32768.0f;
    }
}