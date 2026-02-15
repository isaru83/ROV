#ifndef BALLAST_H
#define BALLAST_H

#include <PID_v1.h>
#include <Arduino.h>
#include "Pins.h"

#define PUMP_FILL_PIN        PIN_DRV_IN1
#define PUMP_EMPTY_PIN       PIN_DRV_IN2

#define PUMP_MIN_DURATION_MS   500     // durée minimale d'impulsion
#define PUMP_MAX_DURATION_MS   5000    // sécurité anti-boucle infinie
#define PUMP_COOLDOWN_MS       8000    // temps minimum entre deux impulsions

class Ballast
{
public:
    Ballast();
    ~Ballast();

    void begin();                           // à appeler dans setup()
    void update(float currentDepth);        // à appeler à chaque loop avec la mesure fraîche

    void setDepthOrder(float targetDepth);
    void setTunings(float Kp, float Ki, float Kd);

    void emergencyStop();                   // arrêt immédiat
    void start();

    float getOutput() const;                // sortie PID actuelle
    bool isPumpActive() const;
    void emptying(bool startFlag);

private:
    double _currentDepth;                   // copie locale (double pour le PID)
    double _targetDepth = 0.0;              // profondeur consigne
    double _pidOutput   = 0.0;              // sortie PID

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