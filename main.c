#include <stdio.h>

#include "pico/stdlib.h"

#include "bts7960.h"

int main() {
    stdio_init_all();

    BTS7960 motor;
    bts7960_init(&motor, 15, 14, 13, 12, 26, 27);

    while (true) {
        // Set motor to move forward at 70% speed
        bts7960_set_direction(&motor, true);
        bts7960_set_speed(&motor, 70);
        bts7960_move(&motor);
        sleep_ms(3000);

        // Read and print current
        float current = bts7960_read_current(&motor);
        printf("Current: %.2f A\n", current);

        // Reverse direction
        bts7960_set_direction(&motor, false);
        bts7960_set_speed(&motor, 50);
        bts7960_move(&motor);
        sleep_ms(3000);

        // Stop the motor
        bts7960_stop(&motor);
        sleep_ms(2000);
    }

    bts7960_cleanup(&motor);
    return 0;
}
