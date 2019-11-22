#include "SoftwareSerial.h"

SoftwareSerial Uno(D6,D7); //Rx, Tx


#define sel1 D3
#define sel2 D4
#define sel3 D5
#define int1 D2

void setup() 
{
  Serial.begin(4800);
  Uno.begin(4800);
  pinMode(sel1,OUTPUT);
  pinMode(sel2,OUTPUT);
  pinMode(sel3,OUTPUT);
  pinMode(int1,OUTPUT);
  digitalWrite(int1,LOW);
}

void loop() 
{
  while(Serial.available())
  {
    
    int p=Serial.parseInt();
    if(p==1)
    {
      digitalWrite(sel1,LOW);
      digitalWrite(sel2,LOW);
      digitalWrite(sel3,LOW);
      Serial.println("sent 000");
      digitalWrite(int1,HIGH);
     
      digitalWrite(int1,LOW);

      while(Uno.available() == 0);

      Serial.print(Uno.readString());

     
    }
    else if(p==2)
    {
      digitalWrite(sel1,HIGH);
      digitalWrite(sel2,LOW);
      digitalWrite(sel3,LOW);
      Serial.println("sent 001");
      digitalWrite(int1,HIGH);
      
      digitalWrite(int1,LOW);
      while(Uno.available() == 0);

      Serial.print(Uno.readString());


      }

     
    }
  }
  
