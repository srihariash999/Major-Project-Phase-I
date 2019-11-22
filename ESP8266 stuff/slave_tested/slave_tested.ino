
const int S0=5;
const int S1=6;
const int S2=7;
const int inter=2;
const int Trigpin=12;
const int Echopin=11;
long duration;
int distance;
void isr();
bool flag;
float recieve_check_type();
void setup() {
Serial.begin(4800);
attachInterrupt(digitalPinToInterrupt(inter),isr,RISING);
 flag=false;
  }


 
  void loop()
  {
    //Serial.println("no interrupt");
//    while(flag==true)
//    {
//      Serial.println("Slave:interrupt called");
//      recieve_check_type();
//      Serial.println("Slave:interrupt ends");
//      flag=false;
//     }
  }
  


  

void isr()
{
//  flag=true;
    
    recieve_check_type();
    return;
   
}

  float recieve_check_type()
{

bool  sel0=digitalRead(S0);
bool  sel1=digitalRead(S1);
bool  sel2=digitalRead(S2);
 
  if(sel0==LOW &&sel1==LOW&&sel2==LOW)
  {float data;
    
     data=ultrasonic();
      
 
 Serial.print(String(data));
 return data;
     
  }
 else if(sel0==HIGH &sel1==LOW&sel2==LOW)
  {
     int other = analogRead(A0);
      
 Serial.print(String(other));
  return other;
  }
  
// 
// Serial.println("data is");
// Serial.println(data);
}

float ultrasonic()
  {
    
    digitalWrite(Trigpin,LOW);
    delayMicroseconds (2);
    digitalWrite (Trigpin,HIGH);
    delayMicroseconds (10);
   digitalWrite(Trigpin,LOW);
   duration=pulseIn(Echopin ,HIGH);
   distance=(duration/2)/29.1;
   
   return distance;
    }
