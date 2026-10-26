// settings.cpp
#include "settings.h" //
#include <EEPROM.h> //

MyAppConfig settings; //

void loadSettings() {
    EEPROM.begin(512);
    EEPROM.get(0, settings);
    if (settings.gifSpeed <= 0 || settings.gifSpeed > 200 || strlen(settings.apName) == 0) {
        settings.gifSpeed = 130;
        settings.gifDelay = 500;
        settings.isInverted = false;
        strcpy(settings.apName, "DASAI");
        strcpy(settings.apPassword, "12345678");
        // Khởi tạo tất cả các GIF được chọn mặc định là true
        for (int i = 0; i < MAX_SELECTED_GIFS; i++) {
            settings.gifSelected[i] = true;
        }
        strcpy(settings.display_type, "ssd1306"); // Mặc định là SSD1306
    }
}
void defaultSettings() {
    settings.gifSpeed = 130;
    settings.gifDelay = 500;
    settings.isInverted = false;
    strcpy(settings.apName, "DASAI");
    strcpy(settings.apPassword, "12345678");

    // Đặt tất cả GIF được chọn mặc định là TRUE
    // Sếp hãy đảm bảo NUM_GIF_ENTRIES hoặc allGifPointers.size() là đúng số lượng GIF nhé.
    for (int i = 0; i < MAX_SELECTED_GIFS; i++) {
        settings.gifSelected[i] = true;
    }

    // Quan trọng: Lưu cài đặt mặc định này vào bộ nhớ non-volatile ngay lập tức
    // để nó không bị mất khi khởi động lại.
    EEPROM.begin(sizeof(settings)); // Hoặc LittleFS, SPIFFS
    EEPROM.put(0, settings);
    EEPROM.commit(); // Ghi vào flash
    EEPROM.end();
    Serial.println("Cài đặt đã được đặt về mặc định và lưu.");
}

void saveSettings() {
    EEPROM.put(0, settings); //
    EEPROM.commit(); //
}