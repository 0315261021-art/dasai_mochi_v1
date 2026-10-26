#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h> 
#include <Wifi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ChronosESP32.h>


BLECharacteristic* pCharacteristic = nullptr;

#include "Dongle_20.h"

#include "vector"
#include "map"
#include "settings.h"
#include "wifi_setup.h"
#include "web_server.h"
#include "gif_data.h"
// INCLUDE file gif tạo từ webapp vô đây
#include "gif/gundam.h"
//#include "gif/daichi_intro.h"
#include "gif/angry.h"
#include "gif/angry2.h"
#include "gif/angry3.h"
#include "gif/aot.h"
#include "gif/bee.h"
#include "gif/cry.h"
#include "gif/cry2.h"
#include "gif/cuoi_khinh_bi.h"
#include "gif/chong_mat.h"
#include "gif/dasai.h"
#include "gif/dasai2.h"
#include "gif/mac_dinh.h"
#include "gif/demon_slayer.h"
#include "gif/den_giao_thong.h"
#include "gif/den_pha.h"
#include "gif/devil.h"
#include "gif/devil_2.h"
#include "gif/devil_eyes.h"
#include "gif/distracted.h"
#include "gif/egg.h"
#include "gif/gian_du.h"
#include "gif/yakura.h"
#include "gif/hand_some.h"
#include "gif/happy_2.h"
#include "gif/hat_xi.h"
#include "gif/hat_xi_2.h"
#include "gif/hehe.h"
#include "gif/intro.h"
#include "gif/keep_it_up.h"
#include "gif/leu_leu.h"
#include "gif/love.h"
#include "gif/love2.h"
#include "gif/mochi.h"
#include "gif/mochidoo.h"
#include "gif/music.h"
#include "gif/neon.h"
#include "gif/noname.h"
#include "gif/nheo_mat.h"
#include "gif/nhin_ben_phai.h"
#include "gif/nhin_ben_trai.h"
#include "gif/nhin_xuong.h"
#include "gif/police.h"
#include "gif/pong.h"
#include "gif/rain.h"
#include "gif/sakura.h"
#include "gif/samurai.h"
#include "gif/scared.h"
#include "gif/sleepy.h"
#include "gif/smile.h"
#include "gif/speed.h"
#include "gif/star.h"
#include "gif/sung_nuoc.h"
#include "gif/sung_nuoc_2.h"
#include "gif/sushi.h"
#include "gif/turbo.h"
#include "gif/thong_long.h"
#include "gif/thu_nho.h"
#include "gif/up_size_down.h"
#include "gif/UwU.h"
#include "gif/wheels.h"
#include "gif/xi_khoi.h"
#include "gif/xi_lua.h"

// Cấu hình màn hình
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 21, /* data=*/ 20);
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* r+eset=*/ U8X8_PIN_NONE, /* clock=*/ 21, /* data=*/ 20);

// Cấu hình Chronos
ChronosESP32 watch("DASAI");
bool change = false;
bool findPhoneRequested = false;
static bool hasNotification = false;
static bool waitingToResumeGif = false;
static bool gifEnabled = true; // Biến này để bật/tắt GIF
static bool forceGif = true; // Biến này để buộc GIF phải tắt
static unsigned long gifStartDelay = 0;
float totalDistance = 0.0;
bool isNavigating = false;
bool useChronos = true;
Navigation nav_iOS;
String replaceCommaWithDot(String str) {
    str.replace(',', '.');
    return str;
}
TaskHandle_t gifTaskHandle = NULL;

// Callback để nhận trạng thái kết nối Chronos
void connectionCallback(bool state)
{
  if(!state){
    WiFi.softAPdisconnect(true);
  }
}
static unsigned long lastNotiTime = 0;
void notificationCallback(Notification notification){    
    if (millis() - lastNotiTime < 500) return; // bỏ qua nếu đến quá nhanh
    hasNotification = true;
    gifEnabled = false;
    u8g2.clearBuffer();        // Xóa sạch khung hình GIF cũ
    u8g2.sendBuffer();
    lastNotiTime = millis();
    if (gifTaskHandle != NULL){
      vTaskSuspend(gifTaskHandle);
      u8g2.clearBuffer();        // Xóa sạch khung hình GIF cũ
      u8g2.sendBuffer();
    }    
    change = true;
    // waitingToResumeGif = true;
    // gifStartDelay = millis();
    delay(100);
}
void configCallback(Config config, uint32_t a, uint32_t b)
{
    switch (config)
    {
    case CF_NAV_DATA:
        Serial.print("Navigation state: ");
        Serial.println(a ? "Active" : "Inactive");
        change = true;
       if (a) { // bắt đầu điều hướng
            if (!isNavigating) {
                Navigation nav = watch.getNavigation();
                String cleanedText = replaceCommaWithDot(nav.distance);
                float distance;
                char unit[4] = "";
                sscanf(cleanedText.c_str(), "%f %3s", &distance, unit);
                if (String(unit).startsWith("k")) distance *= 1000;
                totalDistance = distance;
                isNavigating = true;
            }
        } else {
            isNavigating = false;
        }
        break;
    case CF_NAV_ICON:
        Serial.print("Navigation Icon data, position: ");
        Serial.println(a);
        Serial.print("Icon CRC: ");
        Serial.printf("0x%04X\n", b);
        break;
    case CF_FIND:
        if (millis() - lastNotiTime < 500) return; // bỏ qua nếu đến quá nhanh
        forceGif = !forceGif; // Đảo ngược trạng thái của forceGif
        if(!forceGif){
          gifEnabled = false;
          if (gifTaskHandle != NULL) vTaskSuspend(gifTaskHandle);
        }else{
          gifEnabled = true;
          if (gifTaskHandle != NULL) vTaskResume(gifTaskHandle);          
          waitingToResumeGif = false;
          findPhoneRequested = false;
        }
        lastNotiTime = millis();    
        change = true;
        gifStartDelay = millis();
        delay(100);
        findPhoneRequested = true;
    }
}
const char* textToIcon(const String& text) {
    if (text.indexOf("rẽ trái") >= 0) {
        return "turn_left";
    } else if (text.indexOf("rẽ phải") >= 0) {
        return "turn_right";
    } else if (text.indexOf("quay đầu") >= 0) {
        return "turn_around";
    } else if (text.indexOf("đi thẳng") >= 0) {
        return "straight";
    } else if (text.indexOf("vào vòng xoay") >= 0) {
        return "roundabout";
    } else if (text.indexOf("ra khỏi vòng xoay") >= 0) {
        return "roundabout_exit";
    } else if (text.indexOf("đến nơi") >= 0 || text.indexOf("tới nơi") >= 0) {
        return "arrive";
    } else {
        return text.c_str(); // fallback nếu không khớp
    }
}


void drawDistanceBar(U8G2 &u8g2, const String &distanceText) {
    const int screenWidth = 128;
    const int barHeight = 4;
    const int barY = 64 - barHeight;
    const int borderThickness = 1;

    String cleanedText = replaceCommaWithDot(distanceText);
    float distance = 0;
    char unit[4] = "";
    sscanf(cleanedText.c_str(), "%f %3s", &distance, unit);

    if (String(unit).startsWith("k")) {
        distance *= 1000;
    }

    float ratio = 0;
    if (isNavigating && totalDistance > 0) {
        ratio = distance / totalDistance;
        if (ratio > 1.0) ratio = 1.0;
    }

    int fillWidth = ratio * screenWidth;

    u8g2.drawHLine(0, barY, screenWidth);
    u8g2.drawHLine(0, barY + barHeight - 1, screenWidth);
    u8g2.drawBox(0, barY + borderThickness, fillWidth, barHeight - 2);
}

std::vector<const AnimatedGIF*> allGifPointers = {
  &angry_gif,
  &angry2_gif,
  &angry3_gif,
  &aot_gif,
  &bee_gif,
  &cry_gif,
  &cry2_gif,
  &cuoi_khinh_bi_gif,
  &chong_mat_gif,
  &dasai_gif,
  &dasai2_gif,
  &mac_dinh_gif,
  &demon_slayer_gif,
  &den_giao_thong_gif,
  &den_pha_gif,
  &devil_gif,
  &devil_2_gif,
  &devil_eyes_gif,
  &distracted_gif,
  &egg_gif,
  &gian_du_gif,
  &yakura_gif,
  &hand_some_gif,
  &happy_2_gif,
  &hat_xi_gif,
  &hat_xi_2_gif,
  &hehe_gif,
  &intro_gif,
  &keep_it_up_gif,
  &leu_leu_gif,
  &love_gif,
  &love2_gif,
  &mochi_gif,
  &mochidoo_gif,
  &music_gif,
  &neon_gif,
  &noname_gif,
  &nheo_mat_gif,
  &nhin_ben_phai_gif,
  &nhin_ben_trai_gif,
  &nhin_xuong_gif,
  &police_gif,
  &pong_gif,
  &rain_gif,
  &sakura_gif,
  &samurai_gif,
  &scared_gif,
  &sleepy_gif,
  &smile_gif,
  &speed_gif,
  &star_gif,
  &sung_nuoc_gif,
  &sung_nuoc_2_gif,
  &sushi_gif,
  &turbo_gif,
  &thong_long_gif,
  &thu_nho_gif,
  &up_size_down_gif,
  &UwU_gif,
  &wheels_gif,
  &xi_khoi_gif,
  &xi_lua_gif
};


void playGIF(const AnimatedGIF* gif, uint16_t loopCount = 1) {
  for (uint16_t loop = 0; loop < loopCount; loop++) {
    for (uint8_t frame = 0; frame < gif->frame_count; frame++) {
      // Xóa buffer trước khi vẽ
      u8g2.clearBuffer();

      // Vẽ từng pixel
      for (uint16_t y = 0; y < gif->height; y++) {
        for (uint16_t x = 0; x < gif->width; x++) {
          uint16_t byteIndex = y * (((gif->width + 7) / 8)) + (x / 8);
          uint8_t  bitIndex  = 7 - (x % 8);
          if (gif->frames[frame][byteIndex] & (1 << bitIndex)) {
            u8g2.drawPixel(x, y);
          }
        }
      }

      // Đẩy buffer ra màn hình
      u8g2.sendBuffer();

      // Chờ theo delay của khung
      delay(200-settings.gifSpeed);
    }
  }
}
void playGIFInverted(const AnimatedGIF* gif, uint16_t loopCount = 1) {
  for (uint16_t loop = 0; loop < loopCount; loop++) {
    for (uint8_t frame = 0; frame < gif->frame_count; frame++) {
      u8g2.clearBuffer();

      for (uint16_t y = 0; y < gif->height; y++) {
        for (uint16_t x = 0; x < gif->width; x++) {
          uint16_t byteIndex = y * (((gif->width + 7) / 8)) + (x / 8);
          uint8_t  bitIndex  = 7 - (x % 8);
          
          // ÂM BẢN: vẽ pixel nếu bit bị TẮT thay vì BẬT
          if (!(gif->frames[frame][byteIndex] & (1 << bitIndex))) {
            u8g2.drawPixel(x, y);
          }
        }
      }

      u8g2.sendBuffer();
      delay(200-settings.gifSpeed);
    }
  }
}
void drawWrapped(const String &text, int x, int y, int maxWidth, int lineHeight) {
  int len = text.length();
  std::vector<String> words;
  int idx = 0;

  while (idx < len) {
    int nextSpace = text.indexOf(' ', idx);
    int nextNewline = text.indexOf('\n', idx);

    int nextIdx = -1;
    bool isNewline = false;

    if (nextSpace == -1 && nextNewline == -1) {
      nextIdx = len;
    } else if (nextNewline != -1 && (nextSpace == -1 || nextNewline < nextSpace)) {
      nextIdx = nextNewline;
      isNewline = true;
    } else {
      nextIdx = nextSpace;
    }

    String w = text.substring(idx, nextIdx);
    if (!isNewline && nextIdx < len) w += ' ';  // Thêm space nếu không phải \n

    words.push_back(w);
    if (isNewline) words.push_back("\n");  // Thêm từ đặc biệt xuống dòng

    idx = nextIdx + 1;
  }

  // 2) Vẽ từng dòng, xuống dòng nếu cần
  int cursorY = y;
  String line = "";

  for (auto &w : words) {
    if (w == "\n") {
      // vẽ dòng hiện tại và xuống dòng mới
      u8g2.setCursor(x, cursorY);
      u8g2.print(line);
      cursorY += lineHeight;
      line = "";
    } else if (u8g2.getUTF8Width((line + w).c_str()) <= maxWidth) {
      line += w;
    } else {
      // vẽ dòng hiện tại và bắt đầu dòng mới
      u8g2.setCursor(x, cursorY);
      u8g2.print(line);
      cursorY += lineHeight;
      line = w;
    }
  }

  // Vẽ dòng cuối (nếu còn)
  if (line.length() && cursorY <= u8g2.getDisplayHeight()) {
    u8g2.setCursor(x, cursorY);
    u8g2.print(line);
  }
}
void drawIconAndTitle(U8G2 &u8g2, const Navigation &nav) {
    if (!nav.hasIcon) return;

    // Vẽ icon 48x48 từ mảng bitmap (icon dạng 1bpp)
    u8g2.drawBitmap(0, 0, 48 / 8, 48, nav.icon);

    // Lấy phần đầu của title (trước dấu '/')
    String titleMain = nav.directions;
    int slashIndex = titleMain.indexOf('/');
    if (slashIndex >= 0) {
        titleMain = titleMain.substring(0, slashIndex);
        titleMain.trim(); // bỏ khoảng trắng dư
    }

    // Thiết lập font (sếp thay bằng font khác nếu muốn)
    u8g2.setFont(Dongle_20);

    // Vẽ tiêu đề bên phải icon
    drawWrapped(titleMain, 50, 16, 78, 10); 
}
void drawClockScreen() {
    // Vẽ đồng hồ tìm điện thoại
    String minute = String(watch.getMinute());
    if (minute.length() == 1) {
        minute = "0" + minute; // Thêm số 0 nếu phút chỉ có 1 chữ số
    }
    String second = String(watch.getSecond());
    if (second.length() == 1) {
        second = "0" + second; 
    }
    String timeStr = watch.getHourZ() + ":" + minute + ":" + second;
    String dateStr = String(watch.getDay()) + "/" + String(watch.getMonth());
    String batteryStr = String(watch.getPhoneBattery()) + "%";

    u8g2.clearBuffer();
    u8g2.drawRFrame(0, 0, 128, 64, 4);
    u8g2.setFont(Dongle_20);
    u8g2.drawStr(4, 11, dateStr.c_str());
    int battX = 128 - u8g2.getStrWidth(batteryStr.c_str()) - 4;
    u8g2.drawStr(battX, 11, batteryStr.c_str());
    u8g2.setFont(u8g2_font_logisoso26_tf);
    int tw = u8g2.getStrWidth(timeStr.c_str());
    u8g2.drawStr((128 - tw) / 2, 52, timeStr.c_str());
    u8g2.sendBuffer();
}
String extractCleanDistance(const String& rawText) {
    // Cắt đến ký tự đầu tiên không phải khoảng trắng
    String trimmed = rawText;
    trimmed.trim(); // loại bỏ space, tab, newline ở đầu/cuối

    // Tìm chữ số đầu tiên
    int start = -1;
    for (int i = 0; i < trimmed.length(); i++) {
        if (isDigit(trimmed[i])) {
            start = i;
            break;
        }
    }

    if (start == -1) return "";

    // Tìm vị trí kết thúc của đơn vị (tới khi gặp ký tự không phải chữ hoặc số hoặc khoảng trắng)
    int end = start;
    while (end < trimmed.length() && 
          (isAlphaNumeric(trimmed[end]) || trimmed[end] == ' ' || trimmed[end] == ',' || trimmed[end] == '.')) {
        end++;
    }

    return trimmed.substring(start, end);
}

void shuffleVector(std::vector<const AnimatedGIF*>& vec) {
  int n = vec.size();
  for (int i = n - 1; i > 0; --i) {
    int j = random(i + 1);
    std::swap(vec[i], vec[j]);
  }
}

void gifTask(void *param) {
  while (true) {
    if (gifEnabled) {
      for (const AnimatedGIF* currentGif : allGifPointers) {
        if (settings.isInverted) {
          playGIFInverted(currentGif, 1);
        } else {
          playGIF(currentGif, 1);
        }
        delay(settings.gifDelay); 
      }
    }
  }
}

void setup(void) {
    Serial.begin(115200);
    u8g2.begin();
    u8g2.enableUTF8Print();
    loadSettings();           // Load từ EEPROM
    loadSelectedGifs();
    setupWiFi();              // Tạo WiFi AP
    setupWebServer();         // Web giao diện

    if (useChronos) {
        watch.setConnectionCallback(connectionCallback);
        watch.setNotificationCallback(notificationCallback);
        watch.setConfigurationCallback(configCallback);
        watch.begin();
        Serial.println("BLE Android (Chronos) started");
        watch.setBattery(80);
      } else {
       //IOS
      }
      
    u8g2.clearBuffer();
    u8g2.setFont(Dongle_20);
    u8g2.drawStr(30, 35, "by khanh");
    u8g2.sendBuffer();
    delay(2000);
    shuffleVector(allGifPointers); // Xáo trộn mảng GIF một lần lúc khởi động
    xTaskCreatePinnedToCore(
      gifTask,          // Hàm task
      "gifTask",        // Tên task
      8192,             // Stack size
      NULL,             // Tham số
      1,                // Ưu tiên
      &gifTaskHandle,              // Handle (không cần)
      0                 // Core 0 (WiFi thường ở core 1)
    );

    u8g2.clearBuffer();
    u8g2.sendBuffer(); 

}

void loop(void) {
    handleWeb();
    static unsigned long gifStartDelay = 0;

    watch.loop(); // handles internal routine functions

    if (change){
          change = false;

          Navigation nav = watch.getNavigation();
          if (nav.active){
            if (gifTaskHandle != NULL) {
                vTaskSuspend(gifTaskHandle); // Tạm dừng hiển thị GIF
                gifEnabled = false;
                u8g2.clearBuffer();        // Xóa sạch khung hình GIF cũ
                u8g2.sendBuffer(); 
            }
            u8g2.clearBuffer();
            u8g2.setFont(Dongle_20);
            drawIconAndTitle(u8g2, nav);
            drawDistanceBar(u8g2, nav.distance.c_str());
            //drawWrapped((nav.duration).c_str(), 0, 55, 100, 10); 
            drawWrapped(extractCleanDistance(nav.title).c_str(), 0, 55, 100, 10); // khoảng cách
            u8g2.sendBuffer();
          }else if (findPhoneRequested) {
            drawClockScreen();
            // KHÔNG tắt findPhoneRequested ngay
            if (!waitingToResumeGif) {
                waitingToResumeGif = true;
                gifStartDelay = millis();
            }
            findPhoneRequested = false; // Đặt lại để không lặp lại
          } else if (hasNotification && watch.getNotificationCount() > 0) {
           
            if (!waitingToResumeGif) {
                waitingToResumeGif = true;
                gifStartDelay = millis();
            }

            Notification noti = watch.getNotificationAt(0);
            u8g2.clearBuffer();

          // Vẽ icon nếu muốn (sau này convert int icon thành hình ảnh hoặc ký hiệu)
          // Giả sử vẽ icon tại (0, 0) nếu icon < 10:
          if (noti.icon >= 0 && noti.icon < 10) {
              u8g2.setFont(u8g2_font_open_iconic_embedded_4x_t);
              u8g2.drawGlyph(0, 32, noti.icon); // icon = int, cần tra bảng để biết mã nào là gì
          }
          u8g2.drawRFrame(0, 0, 128, 54, 4);
          // Vẽ tên app
          u8g2.setFont(Dongle_20);
          String msg = noti.message;        
          drawWrapped(msg.c_str(), 2, 11, 126, 10); // Nội dung thông báo

          // Tiêu đề không cần nên bỏ qua
          // u8g2.drawStr(0, 50, noti.title.c_str());
          // Tên APP
          u8g2.drawStr(2, 64, noti.app.c_str());

          // Vẽ thời gian ở góc dưới
          u8g2.setFont(u8g2_font_5x8_tr);
          int tx = 128 - u8g2.getStrWidth(noti.time.c_str()) - 2;
          u8g2.drawStr(tx, 64, noti.time.c_str());

          u8g2.sendBuffer();

          hasNotification = false;
              
        }
      }
    if(waitingToResumeGif && millis() - gifStartDelay > 3000){
      waitingToResumeGif = false;
      if (forceGif){
        if (gifTaskHandle != NULL) vTaskResume(gifTaskHandle);     
        gifEnabled = true;
        findPhoneRequested = false;
      }
    }

    if (!hasNotification && !gifEnabled && !watch.getNavigation().active && !waitingToResumeGif) {
      drawClockScreen(); // liên tục cập nhật đồng hồ
      delay(100); // cập nhật mỗi giây
    }
}

