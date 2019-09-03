#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
 
 
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
 
<h2>Circuits4you<h2>
<h3> HTML Form ESP8266</h3>
 
<form action="/action_page">
  First name:<br>
  <input type="text" name="Node identifier" value="MAP-S-CON-XXXX">
  <br>
  Last name:<br>
  <input type="text" name="Senor to set" value="eg:Temperature.">
  <br><br>
  <input type="submit" value="Submit">
</form> 
 
</body>
</html>
)=====";

 
String nodeIdentifier, sensorUsing = "NaN";
//SSID and Password of your WiFi router
const char* ssid = "SMVDUWIFI";
const char* password = "networkcentre";
 
WebServer server(80);
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
//===============================================================
// This routine is executed when you press submit
//===============================================================
void handleForm() {
 nodeIdentifier = server.arg("Node identifier"); 
 sensorUsing = server.arg("Senor to set"); 
 
 Serial.print("Node in use:");
 Serial.println(nodeIdentifier);
 
 Serial.print("Sensor in use:");
 Serial.println(sensorUsing);
 
 String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send web page
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
