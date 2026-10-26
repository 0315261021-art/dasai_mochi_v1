// settings.h
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <vector> //

// Định nghĩa một kích thước tối đa cho mảng GIF được chọn,
// hoặc một cách để lưu trạng thái của từng GIF (đã chọn/chưa chọn)
#define MAX_SELECTED_GIFS 60 // Ví dụ: số lượng GIF tối đa mà sếp có (tầm 60 theo list của sếp)
// Cấu trúc để lưu trạng thái chọn của từng GIF
struct GifSelection {
    char name[32]; // Tên GIF, ví dụ "angry"
    bool selected;
};

struct MyAppConfig {
    int gifSpeed;
    int gifDelay; // Có vẻ không dùng đến
    bool isInverted;
    bool gifSelected[MAX_SELECTED_GIFS]; // Mảng đánh dấu GIF nào được chọn
    char apName[32];
    char apPassword[32];
    char display_type[10]; // "ssd1306" hoặc "sh1106"
};

extern MyAppConfig settings; //

void loadSettings(); //
void saveSettings(); //
void defaultSettings();
#endif