// ESC.h
#ifndef ESC_H
#define ESC_H

#include <Arduino.h>
#include <HardwareTimer.h>

#define ESC_MIN_US     1000
#define ESC_MAX_US     2000
#define ESC_NEUTRAL_US 1500

class ESC {
public:
    ESC(uint8_t pin, uint16_t minUs = 1000, uint16_t maxUs = 2000, uint16_t neutralUs = 1500);
    ~ESC();

    bool begin();                     
    void write(uint16_t us);          
    void writePercent(float percent); 
    void arm();                       
    void disarm();                    

private:
    uint8_t _pin;
    uint16_t _minUs, _maxUs, _neutralUs;
    uint8_t _channel;                 // 1 à 4 selon le timer
    static HardwareTimer* timer;      // partagé par toutes les instances
    static bool timerInitialized;
    static uint8_t nextChannel;
};

#endif