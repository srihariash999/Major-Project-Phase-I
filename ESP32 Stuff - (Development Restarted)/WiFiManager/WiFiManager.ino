
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

  #include"CS2.h"
#include"Namaste2.h"
#include"ConfigSuccess1.h"

#include "XT_DAC_Audio.h";
WebServer server(80);


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
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
Serial.println(WiFi.localIP());
    DacAudio.PlayWav(&CS2D);
       delay(3000);
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
      
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
  server.handleClient();
  delay(1);
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
