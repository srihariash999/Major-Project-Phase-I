


// Development currently on Hiatus for ESP32 branch. 
//Uncomment these lines if status changed in future.



#include <WiFi.h>

#include <WiFiManager.h>


//#include <ESP8266WebServer.h>     // For ESP8266 uncomment this line.


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




// Some global variables I used.

String nodeIdentifier, sensorUsing = "NaN";







// ----------------------------------- Root handler routine ------------------------------





WebServer server(80);
void setup() {

  
    Serial.begin(115200);

    WiFiManager wifiManager;
     
    //wifiManager.resetSettings();
    //wifiManager.autoConnect("AutoConnectAP");
    wifiManager.autoConnect("Map.S.CON 3000-Setup", "ilovestartrek");
    Serial.println("connected...yeey :)");

    WiFiClient client;
    Serial.print("Connected to ");
    Serial.println("WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); 

   


    
}

void loop() {
  

 server.on("/", handleRoot);             //Which routine to handle at root location
    server.on("/action_page", handleForm);  //form action is handled here
 
    server.begin();                         //Start server
    Serial.println("HTTP server started for Setting the sensor");

    if(nodeIdentifier == "Nan" || sensorUsing == "Nan")
    {
      server.handleClient();  
    }

}




void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}



// ----------------------------------- HTML form handler routine -------------------------


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
