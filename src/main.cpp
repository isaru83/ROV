#include <Arduino.h>
#include "SubMarine.h"

void setup() 
{
  Serial.begin(115200);
  delay(3000);
  Serial2.begin(115200);     
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, HIGH);
}

bool ledFlag = false;
void loop() 
{
  Serial.println("Hello World!!");
  if(Serial.available())
  {
    char c  = Serial.read();
    if(c == 'z')
    {
      if(!ledFlag)
      {
        digitalWrite(PC13, LOW);
        ledFlag = true;
      }
      else
      {
        digitalWrite(PC13, HIGH);
        ledFlag = false;
      }
    }
  }
  delay(200);
  Serial.println("Ca marche enfin!!!");
}
