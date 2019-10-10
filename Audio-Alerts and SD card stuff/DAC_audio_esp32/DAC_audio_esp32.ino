

#include"CS2.h"
#include"Namaste2.h"
#include "XT_DAC_Audio.h";

    
                 
XT_Wav_Class Namaste2D(Namaste2);
XT_Wav_Class CS2D(ConnectionSuccess2);                   
XT_DAC_Audio_Class DacAudio(25,0);    // Create the main player class object. 
                                      // Use GPIO 25, one of the 2 DAC pins and timer 0

void setup() {
  Serial.begin(115200); 

     DacAudio.PlayWav(&Namaste2D);
     delay(1500);
     DacAudio.PlayWav(&CS2D);
      
}

void loop() {

  static uint32_t i=0;                // simple counter to output
 

     delay(5000);
  Serial.println(i);                  // print out the value of i
  i++;  
 
} 
