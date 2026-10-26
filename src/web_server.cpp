// web_server.cpp
#include <WebServer.h>
#include "settings.h"
#include "gif_data.h"

extern std::vector<const AnimatedGIF*> allGifPointers;

WebServer server(80);

// Hàm tạo phần đầu HTML chung
String getHeader(const String& title) {
    String header = "<!DOCTYPE html>";
    header += "<html lang=\"vi\">";
    header += "<head>";
    header += "<meta charset=\"UTF-8\">"; 
    header += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    header += "<title>" + title + "</title>";
    header += "<style>";
    header += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f4f7f6; color: #333; margin: 0; padding: 20px; line-height: 1.6; }";
    header += "h1, h2 { color: #0056b3; border-bottom: 2px solid #0056b3; padding-bottom: 10px; margin-bottom: 20px; }";
    header += "form { background-color: #ffffff; padding: 30px; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 600px; margin: 20px auto; }";
    header += "label { margin-bottom: 8px; font-weight: bold; }";
    header += "input[type=\"text\"], input[type=\"password\"], input[type=\"range\"] { width: calc(100% - 22px); padding: 10px; margin-bottom: 15px; border: 1px solid #ddd; border-radius: 4px; box-sizing: border-box; }";
    header += "input[type=\"radio\"], input[type=\"checkbox\"] { margin-right: 10px; }";
    header += "input[type=\"submit\"] { background-color: #28a745; color: white; padding: 12px 20px; border: none; border-radius: 4px; cursor: pointer; font-size: 16px; margin-top: 20px; transition: background-color 0.3s ease; }";
    header += "input[type=\"submit\"]:hover { background-color: #218838; }";
    header += "ul { list-style-type: none; padding: 0; }";
    header += "li { margin-bottom: 10px; }";
    header += "a { color: #007bff; text-decoration: none; }";
    header += "a:hover { text-decoration: underline; }";
    header += ".message { background-color: #d4edda; color: #155724; border: 1px solid #c3e6cb; padding: 15px; border-radius: 5px; margin: 20px auto; max-width: 600px; text-align: center; }";
    header += "</style>";
    header += "</head>";
    header += "<body>";
    return header;
}

// Hàm tạo phần chân HTML chung
String getFooter() {
    String footer = "</body>";
    footer += "<p class='footer'>2025 DASAI CLONE © Huykhong</p>";
    footer += "</html>";
    return footer;
}

void handleRoot() {
    String html = getHeader("Your DASAI");
    html += "<h1>˃ 𖥦 ˂ WELCOME ˃ 𖥦 ˂</h1>";
    html += "<p>Chọn tùy chọn cấu hình:</p>";
    html += "<ul>";
    html += "<li><a href=\"/gif\">Cấu hình GIF</a></li>";
    html += "<li><a href=\"/wifi\">Cấu hình WiFi</a></li>";
    html += "<li><a href=\"/hardware\">Cấu hình phần cứng</a></li>";
    html += "<li><a href=\"/set_default\">Đặt lại mặc định</a></li>";
    html += "</ul>";
    html += getFooter();
    server.send(200, "text/html", html);
}

void handleGifConfig() {
    String html = getHeader("Cấu hình GIF DASAI CLONE");
    html += "<h1>Cấu hình GIF</h1>";
    html += "<form action=\"/saveGifConfig\" method=\"post\">";

    // Tốc độ GIF
    html += "<label for=\"gifSpeed\">Tốc độ GIF: <span id=\"gifSpeedValue\">" + String(settings.gifSpeed) + "</span></label>";
    html += "<input type=\"range\" min=\"10\" max=\"200\" value=\"" + String(settings.gifSpeed) + "\" name=\"gifSpeed\" id=\"gifSpeed\" oninput=\"document.getElementById('gifSpeedValue').innerText = this.value;\"><br>";

    html += "<label for=\"gifDelay\">Delay mỗi lần đổi GIF (ms): <span id=\"gifDelayValue\">" + String(settings.gifDelay) + "</span></label>";
    html += "<input type=\"range\" min=\"10\" max=\"5000\" value=\"" + String(settings.gifDelay) + "\" name=\"gifDelay\" id=\"gifDelay\" oninput=\"document.getElementById('gifDelayValue').innerText = this.value;\"><br>";
    html += "<p>Mỗi 1000ms = 1s</p>";
    
    // Chế độ âm bản
    html += "<h2>Chế độ hiển thị:</h2>";
    html += "<input type=\"radio\" name=\"inverted\" value=\"false\" id=\"normalMode\" " + String(settings.isInverted ? "" : "checked") + "><label for=\"normalMode\"> Bình thường</label><br>";
    html += "<input type=\"radio\" name=\"inverted\" value=\"true\" id=\"invertedMode\" " + String(settings.isInverted ? "checked" : "") + "><label for=\"invertedMode\"> Âm bản</label><br>";

    // Chọn GIF hiển thị
    html += "<h2>Chọn GIF hiển thị:</h2>";
    for (size_t i = 0; i < MAX_SELECTED_GIFS; i++) {
        String gifName;
        // Logic để gán tên mô tả cho từng GIF
        if (allGifPointers[i] == &angry_gif) gifName = "Angry GIF";
        else if (allGifPointers[i] == &angry2_gif) gifName = "Angry 2 GIF";
        else if (allGifPointers[i] == &angry3_gif) gifName = "Angry 3 GIF";
        else if (allGifPointers[i] == &aot_gif) gifName = "Attack on Titan GIF";
        else if (allGifPointers[i] == &bee_gif) gifName = "Bee GIF";
        else if (allGifPointers[i] == &cry_gif) gifName = "Cry GIF";
        else if (allGifPointers[i] == &cry2_gif) gifName = "Cry 2 GIF";
        else if (allGifPointers[i] == &cuoi_khinh_bi_gif) gifName = "Cười khinh bỉ GIF";
        else if (allGifPointers[i] == &chong_mat_gif) gifName = "Chóng mặt GIF";
        else if (allGifPointers[i] == &dasai_gif) gifName = "Dasai GIF";
        else if (allGifPointers[i] == &dasai2_gif) gifName = "Dasai 2 GIF";
        else if (allGifPointers[i] == &mac_dinh_gif) gifName = "Mặc định GIF";
        else if (allGifPointers[i] == &demon_slayer_gif) gifName = "Demon Slayer GIF";
        else if (allGifPointers[i] == &den_giao_thong_gif) gifName = "Đèn giao thông GIF";
        else if (allGifPointers[i] == &den_pha_gif) gifName = "Đèn pha GIF";
        else if (allGifPointers[i] == &devil_gif) gifName = "Devil GIF";
        else if (allGifPointers[i] == &devil_2_gif) gifName = "Devil 2 GIF";
        else if (allGifPointers[i] == &devil_eyes_gif) gifName = "Devil Eyes GIF";
        else if (allGifPointers[i] == &distracted_gif) gifName = "Distracted GIF";
        else if (allGifPointers[i] == &egg_gif) gifName = "Egg GIF";
        else if (allGifPointers[i] == &gian_du_gif) gifName = "Giận dữ GIF";
        else if (allGifPointers[i] == &yakura_gif) gifName = "Yakura GIF";
        else if (allGifPointers[i] == &hand_some_gif) gifName = "Hand Some GIF";
        else if (allGifPointers[i] == &happy_2_gif) gifName = "Happy 2 GIF";
        else if (allGifPointers[i] == &hat_xi_gif) gifName = "Hắt xì GIF";
        else if (allGifPointers[i] == &hat_xi_2_gif) gifName = "Hắt xì 2 GIF";
        else if (allGifPointers[i] == &hehe_gif) gifName = "Hehe GIF";
        else if (allGifPointers[i] == &intro_gif) gifName = "Intro GIF";
        else if (allGifPointers[i] == &keep_it_up_gif) gifName = "Keep It Up GIF";
        else if (allGifPointers[i] == &leu_leu_gif) gifName = "Lêu lêu GIF";
        else if (allGifPointers[i] == &love_gif) gifName = "Love GIF";
        else if (allGifPointers[i] == &love2_gif) gifName = "Love 2 GIF";
        else if (allGifPointers[i] == &mochi_gif) gifName = "Mochi GIF";
        else if (allGifPointers[i] == &mochidoo_gif) gifName = "Mochidoo GIF";
        else if (allGifPointers[i] == &music_gif) gifName = "Music GIF";
        else if (allGifPointers[i] == &neon_gif) gifName = "Neon GIF";
        else if (allGifPointers[i] == &noname_gif) gifName = "Noname GIF";
        else if (allGifPointers[i] == &nheo_mat_gif) gifName = "Nheo mắt GIF";
        else if (allGifPointers[i] == &nhin_ben_phai_gif) gifName = "Nhìn bên phải GIF";
        else if (allGifPointers[i] == &nhin_ben_trai_gif) gifName = "Nhìn bên trái GIF";
        else if (allGifPointers[i] == &nhin_xuong_gif) gifName = "Nhìn xuống GIF";
        else if (allGifPointers[i] == &police_gif) gifName = "Police GIF";
        else if (allGifPointers[i] == &pong_gif) gifName = "Pong GIF";
        else if (allGifPointers[i] == &rain_gif) gifName = "Rain GIF";
        else if (allGifPointers[i] == &sakura_gif) gifName = "Sakura GIF";
        else if (allGifPointers[i] == &samurai_gif) gifName = "Samurai GIF";
        else if (allGifPointers[i] == &scared_gif) gifName = "Scared GIF";
        else if (allGifPointers[i] == &sleepy_gif) gifName = "Sleepy GIF";
        else if (allGifPointers[i] == &smile_gif) gifName = "Smile GIF";
        else if (allGifPointers[i] == &speed_gif) gifName = "Speed GIF";
        else if (allGifPointers[i] == &star_gif) gifName = "Star GIF";
        else if (allGifPointers[i] == &sung_nuoc_gif) gifName = "Súng nước GIF";
        else if (allGifPointers[i] == &sung_nuoc_2_gif) gifName = "Súng nước 2 GIF";
        else if (allGifPointers[i] == &sushi_gif) gifName = "Sushi GIF";
        else if (allGifPointers[i] == &turbo_gif) gifName = "Turbo GIF";
        else if (allGifPointers[i] == &thong_long_gif) gifName = "Thông lồng GIF";
        else if (allGifPointers[i] == &thu_nho_gif) gifName = "Thu nhỏ GIF";
        else if (allGifPointers[i] == &up_size_down_gif) gifName = "Up Size Down GIF";
        else if (allGifPointers[i] == &UwU_gif) gifName = "UwU GIF";
        else if (allGifPointers[i] == &wheels_gif) gifName = "Wheels GIF";
        else if (allGifPointers[i] == &xi_khoi_gif) gifName = "Xì khói GIF";
        else if (allGifPointers[i] == &xi_lua_gif) gifName = "Xì lụa GIF";
        else gifName = "Unknown GIF"; // Trường hợp không tìm thấy, nên thêm để tránh lỗi

        // Thêm checkbox và label với tên GIF
        html += "<input type=\"checkbox\" name=\"selectedGif\" value=\"" + String(i) + "\" id=\"gif" + String(i) + "\" " + (settings.gifSelected[i] ? "checked" : "") + "><label for=\"gif" + String(i) + "\"> " + gifName + "</label><br>";
    }

    
    html += "<br><input type=\"submit\" value=\"Lưu cấu hình GIF\">";
    html += "</form>";
    html += "<p><a href=\"/\">Quay về trang chủ (Không lưu)</a></p>";
    html += getFooter();
    server.send(200, "text/html", html);
}

void handleSaveGifConfig() {
    if (server.hasArg("gifSpeed")) {
        settings.gifSpeed = server.arg("gifSpeed").toInt();
    }
    if (server.hasArg("gifDelay")) {
        settings.gifDelay = server.arg("gifDelay").toInt();
    }
    if (server.hasArg("inverted")) {
        settings.isInverted = (server.arg("inverted") == "true");
    }

    for (int i = 0; i < MAX_SELECTED_GIFS; i++) {
        settings.gifSelected[i] = false;
    }

    // Lặp qua tất cả các đối số được gửi và kiểm tra xem có phải là "selectedGif" không
    for (int i = 0; i < server.args(); i++) {
        String argName = server.argName(i);
        if (argName == "selectedGif") {
            int gifIndex = server.arg(i).toInt();
            if (gifIndex >= 0 && gifIndex < MAX_SELECTED_GIFS) {
                settings.gifSelected[gifIndex] = true;
            }
        }
    }
    
    saveSettings();
    String html = getHeader("Đã lưu cấu hình GIF!");
    html += "<div class=\"message\"><h1>Đã lưu cấu hình GIF thành công!</h1><p><a href=\"/gif\">Quay lại cấu hình GIF</a></p><p><a href=\"/\">Quay về trang chủ</a></p></div>";
    html += getFooter();
    server.send(200, "text/html", html);
}

void handleWifiConfig() {
    String html = getHeader("Cấu hình WiFi");
    html += "<h1>Cấu hình WiFi</h1>";
    html += "<p>CẨN THẬN VỚI THÔNG TIN CỦA BẠN, NẾU BẠN QUÊN MẬT KHẨU LÀ LỚN CHUYỆN</p>";
    html += "<form action=\"/saveWifiConfig\" method=\"post\">";
    html += "<label for=\"apName\">SSID (Tên điểm truy cập):</label>";
    html += "<input type=\"text\" id=\"apName\" name=\"apName\" value=\"" + String(settings.apName) + "\"><br>";
    html += "<label for=\"apPassword\">Mật khẩu:</label>";
    html += "<input type=\"password\" id=\"apPassword\" name=\"apPassword\" value=\"" + String(settings.apPassword) + "\"><br>";
    // Nhập lại mât khẩu    
    html += "<label for=\"apPasswordConfirm\">Nhập lại mật khẩu:</label>";
    html += "<input type=\"password\" id=\"apPasswordConfirm\" name=\"apPasswordConfirm\" value=\"" + String(settings.apPassword) + "\"><br>";
    html += "<input type=\"submit\" value=\"Lưu cấu hình WiFi\">";
    html += "</form>";
    html += "<p><a href=\"/\">Quay về trang chủ (Không lưu)</a></p>";
    html += getFooter();
    server.send(200, "text/html", html);
}
void handleHardwareConfig() {
    String html = getHeader("Cấu hình phần cứng");
    html += "<h1>Cấu hình phần cứng</h1>";
    html += "<form action=\"/hardware_submit\" method=\"post\">";
    html += "<h2>Chọn loại màn hình OLED</h2>";
    html += "<input type=\"radio\" id=\"display_ssd1306\" name=\"display_type\" value=\"ssd1306\" checked>";
    html += "<label for=\"display_ssd1306\">SSD1306</label><br>";
    html += "<input type=\"radio\" id=\"display_sh1106\" name=\"display_type\" value=\"sh1106\">";
    html += "<label for=\"display_sh1106\">SH1106</label><br><br>";
    //html += "<input type=\"submit\" value=\"Lưu cấu hình phần cứng\">";
    html += "</form>";
    html += "<p><a href=\"/\">Quay về trang chủ (Không lưu)</a></p>";
    html += getFooter();
    server.send(200, "text/html", html);
}
void handleSetDefault() {
    String html = getHeader("Đặt lại cài đặt mặc định");
    html += "<h1>Đặt lại cài đặt</h1>";
    html += "<form action=\"/set_default_submit\" method=\"get\">";
    html += "<p>Đặt lại toàn bộ về mặc định (Tên, passwifi, tuỳ chọn liên kết điện thoại và GIF)?</p>";
    html += "<input type=\"submit\" value=\"Đặt lại\">";
    html += "</form>";
    html += "<p><a href=\"/\">Quay về trang chủ</a></p>";
    html += getFooter();
    server.send(200, "text/html", html);
}
void handleSetDefaultSubmit() {
    defaultSettings();
    String html = getHeader("Đã đặt lại cài đặt mặc định!");
    html += "<div class=\"message\"><h1>Đã đặt lại cài đặt mặc định thành công, nếu chưa thấy gì thì hãy khởi động lại thiết bị!</h1><p><a href=\"/\">Quay về trang chủ</a></p></div>";
    html += getFooter();
    server.send(200, "text/html", html);
}

void handle_hardware_submit() {
    if (server.hasArg("display_type")) {
        // Lấy giá trị display_type từ form và lưu vào settings
        String displayTypeStr = server.arg("display_type");
        strncpy(settings.display_type, displayTypeStr.c_str(), sizeof(settings.display_type) - 1);
        settings.display_type[sizeof(settings.display_type) - 1] = '\0'; // Đảm bảo chuỗi kết thúc bằng null

        saveSettings();

        String html = getHeader("Đã lưu thiết lập phần cứng!");
        html += "<div class=\"message\"><h1>Đã thiết lập cấu hình phần cứng thành công, nếu chưa thấy gì thì hãy khởi động lại thiết bị!</h1><p><a href=\"/\">Quay về trang chủ</a></p></div>";
        html += getFooter();
        server.send(200, "text/html", html);
    }
}


void handleSaveWifiConfig() {
    if (server.hasArg("apName")) {
        strncpy(settings.apName, server.arg("apName").c_str(), sizeof(settings.apName) - 1);
        settings.apName[sizeof(settings.apName) - 1] = '\0';
    }
    if (server.hasArg("apPassword")) {
        // Kiểm tra mật khẩu xác nhận
        if (server.arg("apPassword") != server.arg("apPasswordConfirm")) {
            String html = getHeader("Lỗi xác nhận mật khẩu");
            html += "<div class=\"message\"><h1>Mật khẩu xác nhận không khớp!</h1><p>Vui lòng thử lại.</p><p><a href=\"/wifi\">Quay lại cấu hình WiFi</a></p></div>";
            html += getFooter();
            server.send(200, "text/html", html);
            return;
        }
        // Lưu mật khẩu
        strncpy(settings.apPassword, server.arg("apPassword").c_str(), sizeof(settings.apPassword) - 1);
        settings.apPassword[sizeof(settings.apPassword) - 1] = '\0';
    }
    saveSettings();
    String html = getHeader("Đã lưu cấu hình WiFi!");
    html += "<div class=\"message\"><h1>Đã lưu cấu hình WiFi thành công!</h1><p>Vui lòng khởi động lại thiết bị để áp dụng thay đổi.</p><p><a href=\"/wifi\">Quay lại cấu hình WiFi</a></p><p><a href=\"/\">Quay về trang chủ</a></p></div>";
    html += getFooter();
    server.send(200, "text/html", html);
}

void setupWebServer() {
    server.on("/", handleRoot);
    server.on("/gif", handleGifConfig);
    server.on("/saveGifConfig", handleSaveGifConfig);
    server.on("/wifi", handleWifiConfig);
    server.on("/saveWifiConfig", handleSaveWifiConfig);
    server.on("/set_default", handleSetDefault);
    server.on("/set_default_submit", handleSetDefaultSubmit);
    server.on("/hardware", handleHardwareConfig);
    server.on("/hardware_submit", handle_hardware_submit);

    server.begin();
    Serial.println("Web server started");
}

void handleWeb() {
    server.handleClient();
}