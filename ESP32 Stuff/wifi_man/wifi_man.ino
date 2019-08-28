 #include <WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    WiFiManager wifiManager;
     
//wifiManager.resetSettings();
    //wifiManager.autoConnect("AutoConnectAP");
   wifiManager.autoConnect("Map.S.CON 3000-Setup", "ilovestartrek");
    Serial.println("connected...yeey :)");
}

void loop() {

    delay(10000);
       WiFiManager wifiManager;
wifiManager.resetSettings();
    ESP.restart();
    
}
