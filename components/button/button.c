#include <stdio.h>
// #include <stdlib.h>
#include <freertos/FreeRTOS.h>
// #include <esp_log.h>
#include <driver/gpio.h>
#include "button.h"


// Định nghĩa các chân GPIO cho các nút nhấn
#define GPIO_UP_PIN    GPIO_NUM_25
#define GPIO_DOWN_PIN  GPIO_NUM_26
#define GPIO_PAUSE_PIN GPIO_NUM_27
#define GPIO_START_PIN GPIO_NUM_32

#define ESP_INTR_FLAG_DEFAULT 0
// Thời gian lần nhấn nút trước đó
static uint32_t last_press_time_up = 0;
static uint32_t last_press_time_down = 0;
static uint32_t last_press_time_pause = 0;
static uint32_t last_press_time_start = 0;
static const uint32_t debounce_delay = 200; // 200ms cho debouncing


static button_callback_t button_callback = NULL;

// Hàm xử lý ngắt khi nhấn nút
static void IRAM_ATTR button_isr_handler(void* arg) {
    uint32_t current_time_pause = xTaskGetTickCount() * portTICK_PERIOD_MS;
    // Nếu thời gian giữa hai lần nhấn lớn hơn debounce_delay thì xử lý
    if ((current_time_pause - last_press_time_pause) > debounce_delay) {
        last_press_time_pause = current_time_pause; 
    if (button_callback != NULL) {
        button_callback();  // Gọi hàm callback khi có ngắt
    }
}
}

// Khởi tạo GPIO
void button_init() {
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

    io_conf.pin_bit_mask = (1ULL << GPIO_UP_PIN) |
                           (1ULL << GPIO_DOWN_PIN) |
                           (1ULL << GPIO_START_PIN);
    gpio_config(&io_conf);
    // gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    // gpio_set_intr_type(GPIO_PAUSE_PIN, GPIO_INTR_NEGEDGE);
    // gpio_isr_handler_add(GPIO_PAUSE_PIN, gpio_isr_handler, (void*) GPIO_PAUSE_PIN);
}

void button_pause_init(button_callback_t callback) {
    // Gán hàm callback từ chương trình chính
    button_callback = callback;

    // Cấu hình chân GPIO của nút nhấn
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,  // Ngắt khi sườn xuống (nhấn nút)
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << GPIO_PAUSE_PIN),
        .pull_up_en = GPIO_PULLUP_ENABLE  // Kích hoạt Pull-up
    };
    gpio_config(&io_conf);

    // Đăng ký dịch vụ ngắt và thêm handler
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_PAUSE_PIN, button_isr_handler, NULL);
}


// Hàm đọc trạng thái nút nhấn
ButtonState get_button_state() {
uint32_t current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
    if (gpio_get_level(GPIO_UP_PIN) == 0) {
        if (current_time - last_press_time_up > debounce_delay) {
            last_press_time_up = current_time;
        return BUTTON_UP;
        }
    }
    else if (gpio_get_level(GPIO_DOWN_PIN) == 0) {
        if (current_time - last_press_time_down > debounce_delay) {
            last_press_time_down = current_time;
        return BUTTON_DOWN;
        }
    }
    else if (gpio_get_level(GPIO_PAUSE_PIN) == 0) {
        if (current_time - last_press_time_pause > debounce_delay) {
            last_press_time_pause = current_time;
        return BUTTON_PAUSE;
        }
    }
    else if (gpio_get_level(GPIO_START_PIN) == 0) {
        if (current_time - last_press_time_start > debounce_delay) {
            last_press_time_pause = current_time;
        return BUTTON_START;
        }
    }
    return BUTTON_NONE;
}

