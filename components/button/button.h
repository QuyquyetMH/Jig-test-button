#ifndef BUTTON_H
#define BUTTON_H

// Kiểu liệt kê trạng thái của các nút nhấn
typedef enum {
    BUTTON_NONE = 0,   // Không có nút nào được nhấn
    BUTTON_UP,         // Nút UP được nhấn
    BUTTON_DOWN,       // Nút DOWN được nhấn
    BUTTON_PAUSE,      // Nút PAUSE được nhấn
    BUTTON_START       // Nút START được nhấn
} ButtonState;

ButtonState get_button_state(void);
// Hàm callback để xử lý ngắt khi nút nhấn được kích hoạt
typedef void (*button_callback_t)(void);
// Khai báo hàm khởi tạo GPIO cho các nút nhấn
void button_init(void);
void button_pause_init(button_callback_t callback);
// Hàm đọc trạng thái của nút nhấn


#endif // BUTTON_H
