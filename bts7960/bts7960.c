#include "bts7960.h"

// Initialize the motor driver
void bts7960_init(BTS7960 *driver, uint rpwm, uint lpwm, uint r_en, uint l_en, uint r_is, uint l_is) {
    driver->gpio_rpwm = rpwm;
    driver->gpio_lpwm = lpwm;
    driver->gpio_r_en = r_en;
    driver->gpio_l_en = l_en;
    driver->gpio_r_is = r_is;
    driver->gpio_l_is = l_is;

    // Initialize GPIO pins
    gpio_init(rpwm);
    gpio_set_function(rpwm, GPIO_FUNC_PWM);
    gpio_init(lpwm);
    gpio_set_function(lpwm, GPIO_FUNC_PWM);
    gpio_init(r_en);
    gpio_set_dir(r_en, GPIO_OUT);
    gpio_init(l_en);
    gpio_set_dir(l_en, GPIO_OUT);

    // Enable the motor driver
    gpio_put(r_en, 1);
    gpio_put(l_en, 1);

    // Get PWM slice numbers
    driver->pwm_slice_r = pwm_gpio_to_slice_num(rpwm);
    driver->pwm_slice_l = pwm_gpio_to_slice_num(lpwm);

    // Set PWM frequency and enable slices
    pwm_set_wrap(driver->pwm_slice_r, 255);
    pwm_set_wrap(driver->pwm_slice_l, 255);
    pwm_set_enabled(driver->pwm_slice_r, true);
    pwm_set_enabled(driver->pwm_slice_l, true);

    driver->current_direction = true; // Default to forward direction
    driver->current_speed = 0;        // Default speed is 0

    // Initialize ADC for current sensing
    adc_init();
    adc_gpio_init(r_is);
    adc_gpio_init(l_is);
}

// Set motor direction
void bts7960_set_direction(BTS7960 *driver, bool forward) {
    driver->current_direction = forward;
}

// Set motor speed
void bts7960_set_speed(BTS7960 *driver, uint8_t speed) {
    if (speed > 100) speed = 100;
    driver->current_speed = (speed * 255) / 100;
}

// Move the motor based on the set direction and speed
void bts7960_move(BTS7960 *driver) {
    uint16_t duty_cycle = driver->current_speed;
    if (driver->current_direction) {
        pwm_set_gpio_level(driver->gpio_rpwm, duty_cycle);
        pwm_set_gpio_level(driver->gpio_lpwm, 0);
    } else {
        pwm_set_gpio_level(driver->gpio_rpwm, 0);
        pwm_set_gpio_level(driver->gpio_lpwm, duty_cycle);
    }
}

// Stop the motor
void bts7960_stop(BTS7960 *driver) {
    pwm_set_gpio_level(driver->gpio_rpwm, 0);
    pwm_set_gpio_level(driver->gpio_lpwm, 0);
}

// Free resources
void bts7960_cleanup(BTS7960 *driver) {
    gpio_put(driver->gpio_r_en, 0);
    gpio_put(driver->gpio_l_en, 0);
}

// Read current from IS pins and return the current in amperes
float bts7960_read_current(BTS7960 *driver) {
    uint16_t adc_value;
    float voltage, current;

    // Select the appropriate ADC channel based on the motor direction
    if (driver->current_direction) {
        adc_select_input(0); // Assuming gpio_r_is is connected to ADC channel 0
    } else {
        adc_select_input(1); // Assuming gpio_l_is is connected to ADC channel 1
    }

    // Read the ADC value (12-bit resolution)
    adc_value = adc_read();

    // Convert the ADC value to a voltage (assuming 3.3V reference)
    voltage = (adc_value / 4095.0f) * 3.3f;

    // Convert the voltage to current (based on BTS7960 specs: 0.13 V/A)
    current = voltage / 0.13f;

    return current;
}
