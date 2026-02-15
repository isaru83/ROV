#include "MyTimer.h"

Timer::Timer(unsigned int delay):_prevTime(0),_delay(delay)
{

}

bool Timer::isTrigged()
{
  unsigned long now = millis();
  if(now - _prevTime > _delay)
  {
    _prevTime = now;
    return true;
  }
  return false;
}

bool Timer::isTrigged(bool reboot)
{
  unsigned long now = millis();
  if(now - _prevTime > _delay)
  {
    if(!reboot)
      _prevTime = now;
    return true;
  }
  return false;
}