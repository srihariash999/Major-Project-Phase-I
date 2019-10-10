



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


const char MAIN_page2[]  = R"=====(
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


const char MAIN_page3[]  = R"=====(
<!DOCTYPE html>
<html>
<body>
 
<h2>tlhIngan maH taHjaj!<h2>
<h3> Poop. Balls. Farts.</h3>
<p> For you non-trekkers, that first sentence means, "Remain Klingon"</p>
 
 
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

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

String ssidC, passwordC = "NaN";
String nodeIdentifier, sensorUsing = "NaN";

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
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

 

   if(ssidC == "NaN" || passwordC == "NaN" || ssidC == "XXXX" || passwordC == "XXXX")
  { String p = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", p); //Send web page
 }


   String p = MAIN_page2;
   server.send(200,"text/html",p);
   
}




void handleRoot2() {
 String p = MAIN_page2; //Read HTML contents
 server.send(200, "text/html", p); //Send web page
}

void handleRoot3() {
 String p = MAIN_page3; //Read HTML contents
 server.send(200, "text/html", p ); //Send web page
}

void handleForm2() {
 nodeIdentifier = server.arg("Node identifier"); 
 sensorUsing = server.arg("Senor to set"); 
 
 Serial.print("Node in use:");
 Serial.println(nodeIdentifier);
 
 Serial.print("Sensor in use:");
 Serial.println(sensorUsing);


   if(ssidC == "NaN" || passwordC == "NaN" || ssidC == "MAP-S-CON-XXXX" || passwordC == "eg:Temperature.")
  { String p = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", p); //Send web page
 }


   String p = MAIN_page3;
   server.send(200,"text/html",p);
   


}







void setup() {
  delay(1000);
  WiFi.disconnect(true);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/action_page", handleForm);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {

   if(ssidC == "NaN" || passwordC == "NaN" || 
      ssidC == "XXXX" || passwordC == "XXXX"||
      nodeIdentifier == "MAP-S-CON-XXXX" || sensorUsing == "eg:Temperature."||
      nodeIdentifier == "NaN" || sensorUsing == "NaN")
  {
server.handleClient();
 }


 else
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
 
  server.on("/", handleRoot3);      //Which routine to handle at root location
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
 server.handleClient();
  
 }



}
