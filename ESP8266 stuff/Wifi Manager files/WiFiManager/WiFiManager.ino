#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include<EEPROM.h>

#ifndef APSSID
#define APSSID "MAP-S-CON 3000 Setup"
#define APPSK  "1234567890"
#endif



/*
====================================================================================================================================
                  The webpage displayed where you can enter the details for the Access point you wanna connect.
====================================================================================================================================
*/

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



/*
====================================================================================================================================
                    Declaration of the Global variables used.
====================================================================================================================================
*/


const char *ssid = APSSID;
const char *password = APPSK;
String ssidC, passwordC = "NaN";
int AddrlenSsid = 99, AddrlenPass = 100, AddrSsid = 101, AddrPass = 150;
int lenSsid=0, lenPass=0;


/*
====================================================================================================================================
                    Instantiation of the objects for the libraries used.
====================================================================================================================================
*/


ESP8266WebServer server(80);



/*
====================================================================================================================================
                    Routine that handles the root page on the server.
====================================================================================================================================
*/

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}



/*
====================================================================================================================================
                   Routine that handles the HTML form where user can enter their details
====================================================================================================================================
*/



void handleForm() {
 ssidC = server.arg("SSID"); 
 passwordC = server.arg("PASSWORD"); 
 
 saveDetails(ssidC,passwordC);
 Serial.print("SSID to connect:");
 Serial.println(ssidC);
 
 Serial.print("password:");
 Serial.println(passwordC);

 String p = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", p); //Send web page
}




/*
====================================================================================================================================
                         The setup part. This executes when the ESP restarts  
====================================================================================================================================
*/


void setup() {
  delay(1000);
  
  Serial.begin(115200);
  
  EEPROM.begin(512);

  handleConnection();
  
}





/*
====================================================================================================================================
                    The loop parts. This runs forever. This is where we connect to WiFi using details provided by user.   
====================================================================================================================================
*/



void loop() {


if(WiFi.status() != WL_CONNECTED   && !checkSavedDetails())
{
   WiFi.begin(ssidC, passwordC);   
  Serial.println("");
 
          long long int timeThen = millis();
  bool flag = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    long long int timeNow = millis();
    if((timeNow - timeThen) > 20000)
    {Serial.print("Taking too long");
      flag = false;
      break;
    }
}                                  
 if(flag == false)
{
  Serial.println("Connection taking too long");
  handleConnection();
}
 
  if(WiFi.status() == WL_CONNECTED)
  {Serial.println("");                  //If connection successful show IP address in serial monitor
  Serial.print("Connected to ");
  Serial.println(ssidC);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());       //IP address assigned to your ESP
  }
}

if(WiFi.status() == WL_CONNECTED)
{
  Serial.println("Start the configuration portal");
  delay(2000);
}

}




void saveDetails(String id, String pas)
{
    EEPROM.write(AddrlenSsid,id.length());
    EEPROM.write(AddrlenPass,pas.length());
    EEPROM.commit();
  
    for(int i=0; i<id.length(); i++)
    {
      EEPROM.write(AddrSsid+i,id[i]);
      EEPROM.commit();
      Serial.print("Wrote: ");
      Serial.print(id[i]);
      Serial.print(" to:");
      Serial.println(AddrSsid+i);
    }

    for(int i=0; i<pas.length(); i++)
    {
      EEPROM.write(AddrPass+i,pas[i]);
      EEPROM.commit();
      Serial.print("Wrote: ");
      Serial.print(pas[i]);
      Serial.print(" to:");
      Serial.println(AddrPass+i);
    }

}






bool checkSavedDetails()
{

 lenSsid = EEPROM.read(AddrlenSsid);
 Serial.println("");
 Serial.print("SSID length is: ");
 Serial.println(lenSsid);
String ssidM = "";
String passwordM = "";
for (int i =0; i<lenSsid; i++)
{char n = EEPROM.read(AddrSsid+i);
  ssidM+=n ;
}


lenPass = EEPROM.read(AddrlenPass);
Serial.print("Pass length is:");
Serial.println(lenPass);
for (int i =0; i<lenPass; i++)
{char l = EEPROM.read(AddrPass+i);
  passwordM+=l ;
}

Serial.print("Password is: ");
Serial.println(passwordM);
WiFi.disconnect(true);

 delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssidM);

  WiFi.begin(ssidM, passwordM);
  long long int timeThen = millis();
  bool flag = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    long long int timeNow = millis();
    if((timeNow - timeThen) > 20000)
    {Serial.print("Taking too long");
      flag = false;
      break;
    }
}

if(WiFi.status() == WL_CONNECTED && flag == true)
{
  Serial.println("Connected to WiFi from memory");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());       //IP address assigned to your ESP
  return true;
}
else if(flag == false)
{
  Serial.println("Can't remember the details bruh!");
  return false;
}

}


void handleConnection()
{
  
  if(checkSavedDetails() == false )
  {Serial.println();
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

else
{
  Serial.println("nahi kuch hua");
}
  
}
