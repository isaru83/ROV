// ESC.cpp
#include "ESC.h"

HardwareTimer* ESC::timer = nullptr;
bool ESC::timerInitialized = false;
uint8_t ESC::nextChannel = 1;

ESC::ESC(uint8_t pin, uint16_t minUs, uint16_t maxUs, uint16_t neutralUs)
    : _pin(pin), _minUs(minUs), _maxUs(maxUs), _neutralUs(neutralUs), _channel(0)
{
    if (!timerInitialized) 
    {
        timer = new HardwareTimer(TIM1);           
        timer->setOverflow(50, HERTZ_FORMAT);      // 50 Hz pour les ESC
        timerInitialized = true;
    }

    if (nextChannel <= 4) 
        _channel = nextChannel++;
    else 
        _channel = 0; // erreur
}

ESC::~ESC() 
{ 
}

bool ESC::begin() 
{
    if (_channel == 0) return false;

    timer->setPWM(_channel, _pin, 50, _neutralUs);
    return true;
}

void ESC::write(uint16_t us) 
{
    if (_channel == 0) return;
    us = constrain(us, _minUs, _maxUs);
    timer->setCaptureCompare(_channel, us, MICROSEC_COMPARE_FORMAT);
}

void ESC::writePercent(float percent) 
{
    percent = constrain(percent, -100.0f, 100.0f);
    uint16_t us = _neutralUs + (percent / 100.0f) * (_maxUs - _neutralUs);
    write(us);
}

void ESC::arm() 
{
    write(_minUs);   // souvent 1000
    delay(1000);
    write(_maxUs);   // souvent 2000
    delay(1000); 
    write(_neutralUs);
    delay(2000);    
}

void ESC::disarm() 
{
    write(0);        
}