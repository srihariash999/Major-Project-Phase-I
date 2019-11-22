


//// Development restarted for ESP32 branch. This is the only branch currently supported.
////Uncomment these lines if status changed in future.
//
//







#include <WiFi.h>
#include <WiFiClient.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
ESP8266WebServer webServer(80);
#else
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
WebServer webServer(80);
#endif

  #include"CS2.h"
#include"Namaste2.h"
#include"ConfigSuccess1.h"

#include "XT_DAC_Audio.h";
WebServer server(80);

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

 XT_Wav_Class Namaste2D(Namaste2);
XT_Wav_Class CS2D(ConnectionSuccess2);                   
XT_Wav_Class ConfigSuccess1D(ConfigSuccess1);                   

XT_DAC_Audio_Class DacAudio(25,0); 



const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
 
<h2>Welcome to Configuration Portal<h2>
<h3> Map.S.Con 3000 Setup</h3>
 
<form action="/action_page">
  First name:<br>
  <input type="text" name="Node identifier" value="MAP-S-CON-XXXX">
  <br>
  Last name:<br>
  <input type="text" name="Sensor to set" value="eg:Temperature.">
  <br><br>
  <input type="submit" value="Submit">
</form> 
 
</body>
</html>
)=====";

 
String nodeIdentifier, sensorUsing = "NaN";
//SSID and Password of your WiFi router
const char* ssid = "MAP.S.CON3000-Setup";
const char* password = "poopypants";
 
//===============================================================
// Setup
//===============================================================







 
void setup(void){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
 
     DacAudio.PlayWav(&Namaste2D);
     delay(1500);


       WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  dnsServer.setTTL(300);
  // set which return code will be used for all other domains (e.g. sending
  // ServerFailure instead of NonExistentDomain will reduce number of queries
  // sent by clients)
  // default is DNSReplyCode::NonExistentDomain
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

  // start DNS server for a specific domain name
  dnsServer.start(DNS_PORT, "picha.com", apIP);
Serial.println(WiFi.localIP());
  // simple HTTP server to see that DNS server is working
  webServer.onNotFound([]() {
    String message = "Hello World!\n\n";
    message += "URI: ";
    message += webServer.uri();

    webServer.send(200, "text/plain", message);
  });
  webServer.begin();
}
      
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================


void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
//===============================================================
// This routine is executed when you press submit
//===============================================================
void handleForm() {
 nodeIdentifier = server.arg("Node identifier"); 
 sensorUsing = server.arg("Sensor to set"); 
 
 Serial.print("Node in use:");
 Serial.println(nodeIdentifier);
 
 Serial.print("Sensor in use:");
 Serial.println(sensorUsing);
   DacAudio.PlayWav(&ConfigSuccess1D);
       delay(3000);
 String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send web page
}
