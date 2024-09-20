#include "valve_control.h"
#include "driver/gpio.h"  // Include ESP32 GPIO driver

#define MOSFET_PIN  GPIO_NUM_2
#define SENSOR_PIN  GPIO_NUM_36
// Variable to store the current state of the valve
static valve_state_t control_valve_state = VALVE_OFF;

// Initialize the valve control system
void valve_control_init(void) {
    // Initialize MOSFET pin as output
    gpio_config_t mosfet_conf;
    mosfet_conf.pin_bit_mask = (1ULL << MOSFET_PIN);
    mosfet_conf.mode = GPIO_MODE_OUTPUT;
    mosfet_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    mosfet_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    mosfet_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&mosfet_conf);
   

    // Initialize sensor pin as input
    gpio_config_t sensor_conf;
    sensor_conf.pin_bit_mask = (1ULL << SENSOR_PIN);
    sensor_conf.mode = GPIO_MODE_INPUT;
    sensor_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    sensor_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    sensor_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&sensor_conf);
   
}

// Function to turn the valve ON
void valve_pullup(void) {
    gpio_set_level(MOSFET_PIN, 1);  // Drive MOSFET
    control_valve_state = VALVE_ON;
}

// Function to turn the valve OFF
void valve_pulldown(void){
    gpio_set_level(MOSFET_PIN, 0);
    control_valve_state = VALVE_OFF;
}

// Function to read the sensor state
// bool read_sensor(void) { 
//     return (bool)gpio_get_level( SENSOR_PIN); // Read sensor pin level
// } 