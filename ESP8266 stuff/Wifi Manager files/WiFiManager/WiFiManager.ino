



const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
 
<h2>Welcome to MAP-S-CON 3000 Setup<h2>
<h3> Enter the credentials of the network you want to connect</h3>
 
<form action="/action_page">
  Enter the SSID:<br>
  <input type="text" name="SSID" value="XXXX">
  <br>
  Enter the Password:<br>
  <input type="text" name="PASSWORD" value="XXXX">
  <br><br>
  <input type="submit" value="Submit">
</form> 
 
</body>
</html>
)=====";



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "MAP-S-CON 3000 Setup"
#define APPSK  "1234567890"
#endif


const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

String ssidC, passwordC = "NaN";



void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}


void handleForm() {
 ssidC = server.arg("SSID"); 
 passwordC = server.arg("PASSWORD"); 
 
 Serial.print("SSID to connect:");
 Serial.println(ssidC);
 
 Serial.print("password:");
 Serial.println(passwordC);

 

 //  if(ssidC == "NaN" || passwordC == "NaN" || ssidC == "XXXX" || passwordC == "XXXX")
 // {
 String p = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", p); //Send web page
// }

}



void setup() {
  delay(1000);
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
 
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/action_page", handleForm);
  server.begin();
  Serial.println("HTTP server started");

   while(ssidC == "NaN" || passwordC == "NaN" || 
      ssidC == "XXXX" || passwordC == "XXXX")
  {
server.handleClient();
 }
WiFi.disconnect(true);


  
}

void loop() {


if(WiFi.status() != WL_CONNECTED)
{
   WiFi.begin(ssidC, passwordC);     //Connect to your WiFi router
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
 
}



}
