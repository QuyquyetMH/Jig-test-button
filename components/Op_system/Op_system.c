#include "driver/gpio.h" 
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "esp_timer.h"
// #include "driver/gptimer.h"   // ESP32 timer library
#include "esp_log.h"
#include "Op_system.h"
#include "lcd_display.h"
#include "valve_control.h"

#define TIMER_DIVIDER 80       // Chia xung, tốc độ đếm là 1MHz (80MHz / 80)
#define TIMER_SCALE (TIMER_BASE_CLK / TIMER_DIVIDER)  // Tốc độ đếm timer tính bằng Hz
#define TIMER_INTERVAL_SEC 5   // Thời gian đợi là 5 giây


// Internal buffer for formatting the display
static char lcd_buffer[20]; 

void lcd_display_setup(void){
     LCD_setCursor(0, 2);
     LCD_writeStr("JIG TEST BUTTON");
     LCD_setCursor(1, 0);
     LCD_writeStr("Count: ");
     LCD_setCursor(2, 0);
     LCD_writeStr("Setup: ");
     LCD_setCursor(3, 0);
     LCD_writeStr("SETUP SYSTEM");
}

// Display the countPress value on the LCD
void lcd_display_countPress(uint16_t countPress) {
    // Format the string to display
    //snprintf(lcd_buffer, sizeof(lcd_buffer), "Count: %ld", countPress);
    snprintf(lcd_buffer, sizeof(lcd_buffer),"%u", countPress);
    // Set cursor to line 1 (row 0, col 0)
    LCD_setCursor( 1, 8);

    // Display the formatted string
    LCD_writeStr(lcd_buffer);
}

// Display the setupPress value on the LCD
void lcd_display_setupPress(uint16_t setupPress) {
    // Format the string to display
    //snprintf(lcd_buffer, sizeof(lcd_buffer), "Setup: %ld", setupPress);
    snprintf(lcd_buffer, sizeof(lcd_buffer),"%u",setupPress);

    // Set cursor to line 2 (row 1, col 0)
    LCD_setCursor (2, 8);

    // Display the formatted string
    LCD_writeStr(lcd_buffer);
}

// Display the current system state on the LCD
void lcd_display_state(system_state_t state) {
    // Clear the second line (row 1)
    LCD_setCursor(3, 0);
    LCD_writeStr("                    ");  // Clear 20 characters

    // Set cursor to line 2 (row 1, col 0)
    LCD_setCursor(3, 0);

    // Display the appropriate state message
    switch (state) {
        case SYSTEM_PAUSE:
            LCD_writeStr("PAUSE");
            break;
        case SYSTEM_SETUP:
            LCD_writeStr("SYSTEM SETUP");
            break;
        case  RUNNING_PITON:
            LCD_writeStr("RUN PISTON");
            break;
        case CHECK_BUTTON_PRESS:
            LCD_writeStr("PRESS BUTTON");
            break;
        case CHECK_BUTTON_RELEASE:
            LCD_writeStr("RELEASE BUTTON");
            break;
        case CLOSE_PITON:
            LCD_writeStr("CLOSE PISTON");
            break;
        case SYSTEM_ERROR_1:
            LCD_writeStr("ERROR 1");
            break;
        case SYSTEM_ERROR_2:
            LCD_writeStr("ERROR 2");
            break;
        default:
            break;
    }
}

void button_check_init(gpio_num_t button_check_num){
    gpio_config_t button_conf;

    button_conf.intr_type = GPIO_INTR_DISABLE;
    button_conf.mode = GPIO_MODE_INPUT;
    button_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    button_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

    button_conf.pin_bit_mask = (1ULL << button_check_num);
   
    gpio_config(&button_conf);
}

bool get_check_button_level(gpio_num_t button_check_num)
{
    return (bool)gpio_get_level(button_check_num);
}
   

// void IRAM_ATTR timer_callback(void* arg) {

//     read_sensor();
//     // Xóa cờ ngắt và kích hoạt lại alarm để ngắt tiếp tục hoạt động bình thường
//     timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
// }

// void check_valve_on(void){
//      valve_pullup();
//      xTimerStart(timeout_timer, 0);
//      start_timer();
// }

// void check_valve_off(void){
//     valve_pulldown();
// }

// #define TIMEOUT_MS 2000  // Timeout sau 2000ms

// void check_valve_on(void){
//      valve_pullup();
// }


