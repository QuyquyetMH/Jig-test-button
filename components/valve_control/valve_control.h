#ifndef VALVE_CONTROL_H
#define VALVE_CONTROL_H


#include <stdint.h>
#include <stdbool.h>


// Example GPIO pin for sensor
typedef enum {
    VALVE_OFF = 0,
    VALVE_ON
} valve_state_t;
static valve_state_t control_valve_state ;
    
// Function to initialize the valve control system
void valve_control_init(void);

// Function to turn the valve ON
void valve_pullup(void);

// Function to turn the valve OFF
void valve_pulldown(void);

// Function to read the sensor state
//bool read_sensor(void);

#endif // VALVE_CONTROL_H
