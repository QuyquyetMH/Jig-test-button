#ifndef FLASH_STORAGE_H
#define FLASH_STORAGE_H

#include <stdint.h>
#include "esp_err.h"

// Hàm khởi tạo Flash
esp_err_t flash_storage_init(void);

// Hàm lưu giá trị vào Flash
esp_err_t flash_save_pressCount(uint16_t pressCount);
esp_err_t flash_save_setupCount(uint16_t setupCount);
esp_err_t flash_save_error1(uint16_t count_error_1);
esp_err_t flash_save_error2(uint16_t count_error_2);
// Hàm đọc giá trị từ Flash
esp_err_t flash_load_pressCount(uint16_t *pressCount);
esp_err_t flash_load_setupCount(uint16_t *setupCount);
esp_err_t flash_load_error1(uint16_t *count_error_1);
esp_err_t flash_load_error2(uint16_t *count_error_2);
#endif // FLASH_STORAGE_H
