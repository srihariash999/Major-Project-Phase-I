
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include<EEPROM.h>
#include <ESP8266HTTPClient.h>

#include "SoftwareSerial.h"

SoftwareSerial Uno(D6,D7); //Rx, Tx


#define sel1 D3
#define sel2 D4
#define sel3 D5
#define int1 D2

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
                  The webpage displayed where you can enter the details of Node and Sensor connected.
====================================================================================================================================
*/


const char MAIN_page2[]  = R"=====(
<!DOCTYPE html>
<html>
<body>
 
<h2>MAP.S.CON 3000 Configuration<h2>
<h3>Select the configuration please.</h3>
 
<form action="/action_page">
  Device ID:<br>
  <input type="text" name="Node identifier" value="MAP-S-CON-XXXX">
  <br>
  Sensor type:<br>
  <input type="text" name="Sensor to set" value="eg:Temperature.">
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
String ssidM,passwordM;
int AddrlenSsid = 99, AddrlenPass = 100, AddrSsid = 101, AddrPass = 150;
int lenSsid=0, lenPass=0;
String nodeIdentifier, sensorUsing = "NaN";
bool configDone = false;

/*
====================================================================================================================================
                    Instantiation of the objects for the libraries used.
====================================================================================================================================
*/


ESP8266WebServer server(80);



/*
====================================================================================================================================
                    Routines that handles the root page on the server.
====================================================================================================================================
*/

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleRoot2() {
 String s = MAIN_page2; //Read HTML contents
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


void handleForm2() {
 nodeIdentifier = server.arg("Node identifier"); 
 sensorUsing = server.arg("Sensor to set"); 
 
 Serial.print("Node in use:");
 Serial.println(nodeIdentifier);
 
 Serial.print("Sensor in use:");
 Serial.println(sensorUsing);
 
 String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send web page
}


/*
====================================================================================================================================
                         The setup part. This executes when the ESP restarts  
====================================================================================================================================
*/


void setup() {
  delay(1000);
  Serial.begin(4800);
  Uno.begin(4800);
  pinMode(sel1,OUTPUT);
  pinMode(sel2,OUTPUT);
  pinMode(sel3,OUTPUT);
  pinMode(int1,OUTPUT);
  digitalWrite(int1,LOW);

  
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


  
  if(configDone == false)
  {
    configPortal();
  }

  if(configDone == true)
  {

    HTTPClient http; 
    if(sensorUsing == "soilmoisture")
   
    {
        
      digitalWrite(sel1,LOW);
      digitalWrite(sel2,LOW);
      digitalWrite(sel3,LOW);
      Serial.println("sent 000");
      digitalWrite(int1,HIGH);
      delay(100);
      digitalWrite(int1,LOW);

      while(Uno.available() == 0);
      String valSens = Uno.readString();

      Serial.print(valSens);

      String pushUrl = "http://mapscon.000webhostapp.com/api/soilmoisture/insert.php?moist=";
      pushUrl += valSens;
      Serial.println("Starting request");
       
      http.begin(pushUrl);  //Specify request destination
      int httpCode = http.GET();                                                                  //Send the request
       
      if (httpCode > 0) { //Check the returning code
       
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
       
      }

      else
      {
        Serial.print(httpCode);
      }
       
      http.end();


      
    }

    else if (sensorUsing == "ir")
    {
      digitalWrite(sel1,HIGH);
      digitalWrite(sel2,LOW);
      digitalWrite(sel3,LOW);
      Serial.println("sent 001");
      digitalWrite(int1,HIGH);
      delay(100);
      digitalWrite(int1,LOW);
      while(Uno.available() == 0);
      String valSens = Uno.readString();

      Serial.print(valSens);

      String pushUrl = "http://mapscon.000webhostapp.com/api/ir/insert.php?irval=";
      pushUrl += valSens;
      Serial.println("Starting request");
       
      http.begin(pushUrl);  //Specify request destination
      int httpCode = http.GET();                                                                  //Send the request
       
      if (httpCode > 0) { //Check the returning code
       
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
       
      }

      else
      {
        Serial.print(httpCode);
      }
       
      http.end();



    }



    
  }
  delay(5000);
}









}           // Loop ends.




void configPortal()
{
//  if(WiFi.status() != WL_CONNECTED)
//  {
//    handleConnection();
//    return;
//  }
 

Serial.print("Configuring access point...");
 
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot2);
  server.on("/action_page", handleForm2);
  server.begin();

  
  Serial.println("HTTP server started");

  while(nodeIdentifier == "NaN" || sensorUsing == "NaN")
  {
  server.handleClient();          //Handle client requests
  }

connectWifiSTA();
configDone = true;
  return;
  
  
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
ssidM = "";
passwordM = "";
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
   WiFi.mode(WIFI_STA);
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
WiFi.mode(WIFI_STA);
  }

else
{
  Serial.println("nahi kuch hua");
}
  
}


void connectWifiSTA()
{

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssidM);
   WiFi.mode(WIFI_STA);
  WiFi.begin(ssidM, passwordM);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
}
 Serial.print("Connected to: ");
 Serial.println(ssidM);
 Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  

  Serial.print("Normal wifi mode switched");

  
}
