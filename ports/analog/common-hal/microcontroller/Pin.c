
#include <stdbool.h>

#include "shared-bindings/microcontroller/Pin.h"
#include "mpconfigboard.h"
#include "pins.h"

#include "mxc_sys.h"
#include "max32690.h"
#include "gpio.h"

void reset_all_pins(void) {
    // todo: this is not a good method for this long-term
    // Pins should be individually reset to account for never_reset pins like SWD
    for (int i = 0; i < NUM_GPIO_PORTS; i++) {
        MXC_GPIO_Reset(i);
    }
}

// todo: Implement
void reset_pin_number(uint8_t pin) {
}

// todo: Implement
void claim_pin(const mcu_pin_obj_t *pin) {
    return;
}

// todo: Implement
bool pin_number_is_free(uint8_t pin_number) {
    return true;
}


// todo: Implement
void never_reset_pin_number(uint8_t pin_number) {
    return;
}

//todo: implement
uint8_t common_hal_mcu_pin_number(const mcu_pin_obj_t *pin) {
    return 0;
}

// todo: implement
bool common_hal_mcu_pin_is_free(const mcu_pin_obj_t *pin) {
    return true;
}

void common_hal_never_reset_pin(const mcu_pin_obj_t *pin) {
}

void common_hal_reset_pin(const mcu_pin_obj_t *pin) {
}

void common_hal_mcu_pin_claim(const mcu_pin_obj_t *pin) {
}

void common_hal_mcu_pin_claim_number(uint8_t pin_no) {
}

void common_hal_mcu_pin_reset_number(uint8_t pin_no) {
}
