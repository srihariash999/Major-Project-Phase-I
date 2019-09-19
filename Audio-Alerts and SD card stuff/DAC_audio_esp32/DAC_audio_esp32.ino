
#include "audio.h";
#include"sound.h";
#include "XT_DAC_Audio.h";

    
XT_Wav_Class stressingD(stressing);                 
XT_Wav_Class rainingD(raining);                    
XT_DAC_Audio_Class DacAudio(25,0);    // Create the main player class object. 
                                      // Use GPIO 25, one of the 2 DAC pins and timer 0

void setup() {
  Serial.begin(115200);
 
     DacAudio.PlayWav(&rainingD);
         delay(5000);
       DacAudio.PlayWav(&stressingD);  // play the wav (pass the wav class object created at top of code


    

   
}

void loop() {

  static uint32_t i=0;                // simple counter to output
 

     delay(5000);
  Serial.println(i);                  // print out the value of i
  i++;  
 
} 
