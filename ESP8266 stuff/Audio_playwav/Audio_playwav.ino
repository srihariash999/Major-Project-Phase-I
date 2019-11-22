#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif

#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"


#include"ConfigSuccess1.h"

AudioGeneratorWAV *wav;
AudioFileSourcePROGMEM *file;
AudioOutputI2SNoDAC *out;

void setup()
{
  //WiFi.mode(WIFI_OFF); 
  Serial.begin(115200);
  delay(1000);
  Serial.printf("WAV start\n");


file = new AudioFileSourcePROGMEM( ConfigSuccess1, sizeof(ConfigSuccess1) );
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);


}

void loop()
{


   
    if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
    
  }
  

}
