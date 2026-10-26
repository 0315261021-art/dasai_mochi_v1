
#include <WiFi.h>
#include <ESPmDNS.h>
#include "settings.h"

void setupWiFi() {
    WiFi.softAP(settings.apName, settings.apPassword);
    delay(100);
    IPAddress myIP = WiFi.softAPIP();
    char ipStr[16];  // Đủ để chứa "xxx.xxx.xxx.xxx\0"
    sprintf(ipStr, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
    Serial.print("AP IP address: ");
    Serial.println(myIP);

      if (!MDNS.begin("dasai")) { // Tên này sẽ thành dasai.local
       Serial.println("mDNS khởi tạo thất bại!");
    } else {
        Serial.println("mDNS đã khởi tạo, có thể truy cập bằng: http://dasai.local");
        // mDNS có thể quảng bá các dịch vụ, ví dụ dịch vụ web HTTP trên cổng 80
        MDNS.addService("http", "tcp", 80);
    }
}
