#include <Arduino.h>
#include "SubMarine.h"

SubMarine sub;

void setup() 
{
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(1000);
  sub.begin();
}

void loop() 
{
  sub.run();
  delay(200);
}
