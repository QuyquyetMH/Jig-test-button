#include "flash_storage.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

//static const char *TAG = "FlashStorage";
static const char *NAMESPACE = "storage";

// Hàm khởi tạo NVS (Flash)
esp_err_t flash_storage_init(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    //ESP_LOGI(TAG, "NVS initialized");
    return ret;
}

// Hàm lưu pressCount
esp_err_t flash_save_pressCount(uint16_t pressCount) {
    nvs_handle_t handle;
    esp_err_t ret = nvs_open(NAMESPACE, NVS_READWRITE, &handle);
    if (ret != ESP_OK) return ret;

    ret = nvs_set_u16(handle, "pressCount", pressCount);
    if (ret == ESP_OK) {
        ret = nvs_commit(handle);
        //ESP_LOGI(TAG, "Saved pressCount = %d", pressCount);
    }
    nvs_close(handle);
    return ret;
}

// Hàm lưu setupCount
esp_err_t flash_save_setupCount(uint16_t setupCount) {
    nvs_handle_t handle;
    esp_err_t ret = nvs_open(NAMESPACE, NVS_READWRITE, &handle);
    if (ret != ESP_OK) return ret;

    ret = nvs_set_u16(handle, "setupCount", setupCount);
    if (ret == ESP_OK) {
        ret = nvs_commit(handle);
        //ESP_LOGI(TAG, "Saved setupCount = %d", setupCount);
    }
    nvs_close(handle);
    return ret;
}

// Hàm đọc pressCount
esp_err_t flash_load_pressCount(uint16_t *pressCount) {
    nvs_handle_t handle;
    esp_err_t ret = nvs_open(NAMESPACE, NVS_READONLY, &handle);
    if (ret != ESP_OK) return ret;

    ret = nvs_get_u16(handle, "pressCount", pressCount);
    if (ret == ESP_OK) {
       // ESP_LOGI(TAG, "Loaded pressCount = %d", *pressCount);
    }
    nvs_close(handle);
    return ret;
}

// Hàm đọc setupCount
esp_err_t flash_load_setupCount(uint16_t *setupCount) {
    nvs_handle_t handle;
    esp_err_t ret = nvs_open(NAMESPACE, NVS_READONLY, &handle);
    if (ret != ESP_OK) return ret;

    ret = nvs_get_u16(handle, "setupCount", setupCount);
    if (ret == ESP_OK) {
       // ESP_LOGI(TAG, "Loaded setupCount = %d", *setupCount);
    }
    nvs_close(handle);
    return ret;
}
