/*******************************************************************************
 *				 _ _                                             _ _
				|   |                                           (_ _)
				|   |        _ _     _ _   _ _ _ _ _ _ _ _ _ _   _ _
				|   |       |   |   |   | |    _ _     _ _    | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |_ _ _  |   |_ _|   | |   |   |   |   |   | |   |
				|_ _ _ _ _| |_ _ _ _ _ _| |_ _|   |_ _|   |_ _| |_ _|
								(C)2023 Lumi
 * Copyright (c) 2023
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File name: 
 *
 * Description: This code is used for tranning Lumi IOT member. It is the code form statandard.
 *
 * Author:
 *
 * Last Changed By:  $Author: 
 * Revision:         $Revision: $
 * Last Changed:     $Date: $Feb 9, 2023
 *
 * Code sample:
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "button.h"
#include "valve_control.h"
#include "Op_system.h"
#include "lcd_display.h"
#include "flash_storage.h"
#include "sdkconfig.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

#define BUTTON_CHECK_PIN   GPIO_NUM_23
#define BUZZER_PIN         GPIO_NUM_4
#define LCD_ADDR 0x27
#define SDA_PIN  19
#define SCL_PIN  18
#define LCD_COLS 20
#define LCD_ROWS 4

#define TIMEOUT_MS 1000  // 1 second timeout in milliseconds

TickType_t start_tick;
TickType_t timeout_ticks = pdMS_TO_TICKS(TIMEOUT_MS);


uint8_t count_error_1, count_error_2;
uint16_t pressCount ;
uint16_t setupCount;
uint8_t buttonState;
bool check_button_state;

system_state_t state = RUNNING_PITON; // trạng thái hiện tại của hệ thống 

typedef enum {
  STATE_EVENT_STARTUP,
  STATE_EVENT_IDLE,
  STATE_EVENT_RUN,
  STATE_EVENT_ERROR
}EventState;

EventState eventCurrentState = STATE_EVENT_STARTUP;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void OperationSystem(void);
void StateManager(void);
static void appInit(void);
void app_main(void);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void pause_button_pressed() {
    // Chuyển trạng thái hệ thống sang SYSTEM_IDLE
    eventCurrentState = STATE_EVENT_IDLE;
    state = SYSTEM_PAUSE;
}

void StateManager() {
    switch (eventCurrentState) {
        case STATE_EVENT_STARTUP:
            pressCount = flash_load_pressCount(&pressCount);
            setupCount = flash_load_setupCount(&setupCount);
            lcd_display_setup();
            lcd_display_setupPress(setupCount);
            lcd_display_countPress(pressCount);
            eventCurrentState = STATE_EVENT_IDLE;
            break;

        case STATE_EVENT_IDLE:
            buttonState = get_button_state();
            while (buttonState == BUTTON_NONE) {
                buttonState = get_button_state();
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
            switch (buttonState) {
                case BUTTON_PAUSE:
                    flash_save_pressCount(pressCount);
                    flash_save_setupCount(setupCount);
                    buttonState = BUTTON_NONE;
                    break;

                case BUTTON_START:
                    eventCurrentState = STATE_EVENT_RUN;
                    break;

                case BUTTON_UP:
                    setupCount += 500;
                    buttonState = BUTTON_NONE;
                    break;

                case BUTTON_DOWN:
                    if (setupCount >= 500) {
                        setupCount -= 500;
                    }
                    buttonState = BUTTON_NONE;
                    break;
            }
            break;
        case STATE_EVENT_RUN:
            if (pressCount <= setupCount) {
                OperationSystem();
            } else {
                eventCurrentState = STATE_EVENT_IDLE;
            }
            break;

        case STATE_EVENT_ERROR:
            valve_pulldown();
            gpio_set_level(BUZZER_PIN, 1);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            flash_save_pressCount(pressCount);
            flash_save_setupCount(setupCount);

            while (buttonState != BUTTON_START) {
                buttonState = get_button_state();
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
            eventCurrentState = STATE_EVENT_IDLE;
            break;

        default:
            break;
    }
}
void OperationSystem(void) {
    switch (state) {
        case RUNNING_PITON:
            lcd_display_state(state);
            valve_pullup();
            if (control_valve_state == VALVE_ON) {
                vTaskDelay(500 / portTICK_PERIOD_MS);
                state = CHECK_BUTTON_PRESS;
            }
            break;

        case CHECK_BUTTON_PRESS:
            lcd_display_state(state);
            start_tick = xTaskGetTickCount();
            while (xTaskGetTickCount() - start_tick < timeout_ticks) {
                check_button_state = get_check_button_level(BUTTON_CHECK_PIN);
                if (check_button_state) {
                    count_error_1 = 0;
                    pressCount++;
                    lcd_display_countPress(pressCount);
                    state = CLOSE_PITON;
                    return;
                }
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
            state = SYSTEM_ERROR_1;
            break;

        case CLOSE_PITON:
            lcd_display_state(state);
            valve_pulldown();
            if (control_valve_state == VALVE_OFF) {
                vTaskDelay(500 / portTICK_PERIOD_MS);
                state = CHECK_BUTTON_RELEASE;
            }
            break;

        case CHECK_BUTTON_RELEASE:
            lcd_display_state(state);
            start_tick = xTaskGetTickCount();
            while (xTaskGetTickCount() - start_tick < timeout_ticks) {
                check_button_state = get_check_button_level(BUTTON_CHECK_PIN);
                if (!check_button_state) {
                    count_error_2 = 0;
                    state = RUNNING_PITON;
                    return;
                }
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
            state = SYSTEM_ERROR_2;
            break;

        case SYSTEM_PAUSE:
            lcd_display_state(state);
            eventCurrentState = STATE_EVENT_IDLE;
            break;

        case SYSTEM_ERROR_1:
            count_error_1++;
            if (count_error_1 > 5) {
                lcd_display_state(state);
                eventCurrentState = STATE_EVENT_ERROR;
            } else {
                state = RUNNING_PITON;
            }
            break;

        case SYSTEM_ERROR_2:
            count_error_2++;
            if (count_error_2 > 5) {
                lcd_display_state(state);
                eventCurrentState = STATE_EVENT_ERROR;
            } else {
                state = CLOSE_PITON;
            }
            break;

        default:
            break;
    }
}

static void appInit(void) {
    button_init();
    button_check_init(BUTTON_CHECK_PIN);
    button_pause_init(pause_button_pressed);
    valve_control_init();
    LCD_init(LCD_ADDR, SDA_PIN, SCL_PIN, LCD_COLS, LCD_ROWS);
    flash_storage_init();
}

void app_main(void) {
    appInit();
    while (true) {
        StateManager();
    }
}

