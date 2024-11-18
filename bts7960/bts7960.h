#pragma once

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

typedef struct {
    uint gpio_rpwm;
    uint gpio_lpwm;
    uint gpio_r_en;
    uint gpio_l_en;
    uint gpio_r_is; // Right current sense pin
    uint gpio_l_is; // Left current sense pin
    uint pwm_slice_r;
    uint pwm_slice_l;
    bool current_direction;
    uint16_t current_speed;
} BTS7960;

// Function prototypes
void bts7960_init(BTS7960 *driver, uint rpwm, uint lpwm, uint r_en, uint l_en, uint r_is, uint l_is);
void bts7960_set_direction(BTS7960 *driver, bool forward);
void bts7960_set_speed(BTS7960 *driver, uint8_t speed);
void bts7960_move(BTS7960 *driver);
void bts7960_stop(BTS7960 *driver);
void bts7960_cleanup(BTS7960 *driver);
float bts7960_read_current(BTS7960 *driver);

