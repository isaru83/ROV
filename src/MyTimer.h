#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "Arduino.h"

class Timer
{
  public:
    Timer(unsigned int delay);
    bool isTrigged();
    bool isTrigged(bool reboot);
    unsigned int getDelay() { return _delay; }
    void reboot() { _prevTime = millis(); }

  private:
    unsigned long _prevTime;
    unsigned int _delay;
};

#endif // TIMER_H_INCLUDED
