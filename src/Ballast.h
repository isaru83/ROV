#ifndef BALLAST_H
#define BALLAST_H

#include <PID_v1.h>
#include <Arduino.h>
#include "Pins.h"

#define PUMP_FILL_PIN        PIN_DRV_IN1
#define PUMP_EMPTY_PIN       PIN_DRV_IN2

#define PUMP_MIN_DURATION_MS   500     
#define PUMP_MAX_DURATION_MS   5000    
#define PUMP_COOLDOWN_MS       8000   

class Ballast
{
public:
    Ballast();
    ~Ballast();

    void begin();                          
    void update(float currentDepth);     

    void setDepthOrder(float targetDepth);
    void setTunings(float Kp, float Ki, float Kd);

    void emergencyStop();                 
    void start();

    float getOutput() const;             
    bool isPumpActive() const;
    void emptying(bool startFlag);

private:
    double _currentDepth;                  
    double _targetDepth = 0.0;              
    double _pidOutput   = 0.0;             

    PID    _pid;
    // Gestion non-bloquante de la pompe
    bool     _pumpActive     = false;
    bool     _pumpFilling    = false;       // true = remplir, false = vider
    uint32_t _pumpStartTime  = 0;
    uint32_t _pumpDuration   = 0;
    uint32_t _lastPumpAction = 0;           // pour le cooldown

    void startPump(bool fill, uint32_t duration_ms);
    void stopPump();
};

#endif