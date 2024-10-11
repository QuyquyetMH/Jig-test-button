#ifndef OP_SYSTEM_H
#define OP_SYSTEM_H

#include <stdint.h>

// Enum for system states
typedef enum {
    SYSTEM_PAUSE = 0,
    SYSTEM_SETUP,
    OPEN_BUTTON,
    CHECK_SENSOR_CURRENT,
    CHECK_SENSOR_NONE,
    CLOSE_BUTTON,
    SYSTEM_ERROR_1,
    SYSTEM_ERROR_2
} system_state_t;

void lcd_display_setup(void);
// Display the countPress value on the LCD
void lcd_display_countPress(uint16_t countPress);

// Display the setupPress value on the LCD
void lcd_display_setupPress(uint16_t setupPress);
void lcd_display_error1(uint16_t count_error_1);
void lcd_display_error2(uint16_t count_error_2);
// Display the current system state (PAUSE, RUN, ERROR 1, 2, 3)
//void lcd_display_state(system_state_t state);

void sensor_check_init(gpio_num_t sensor_check_num);
bool get_check_sensor_level(gpio_num_t sensor_check_num);
void buzzer_init(gpio_num_t buzzer_num);

void start_timer(void);
void check_valve_on(void);
void check_valve_off(void);
// extern bool check_button_state;

#endif