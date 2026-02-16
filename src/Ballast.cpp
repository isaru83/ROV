#include "Ballast.h"

Ballast::Ballast()
    : _pid(&_currentDepth, &_pidOutput, &_targetDepth, 2.0, 0.05, 0.2, REVERSE)
{
    pinMode(PUMP_FILL_PIN,  OUTPUT);
    pinMode(PUMP_EMPTY_PIN, OUTPUT);
    stopPump();
}

Ballast::~Ballast() 
{
    stopPump();
}

void Ballast::begin()
{
    _pid.SetMode(MANUAL);
    _pid.SetOutputLimits(-PUMP_MAX_DURATION_MS, PUMP_MAX_DURATION_MS);
    _pid.SetSampleTime(200);  // mise à jour PID toutes les 200 ms
}

void Ballast::start()
{
    _pid.SetMode(AUTOMATIC);
}

void Ballast::update(float currentDepth)
{
    _currentDepth = static_cast<double>(currentDepth);
    _pid.Compute();

    uint32_t now = millis();

    // Gestion de l'impulsion pompe (non-bloquante)
    if (_pumpActive)
        if (now - _pumpStartTime >= _pumpDuration)
            stopPump();

    // Cooldown entre deux commande
    if (!_pumpActive && (now - _lastPumpAction < PUMP_COOLDOWN_MS))
        return;

    // Pas en cooldown et pas en commandé
    if (!_pumpActive && (now - _lastPumpAction >= PUMP_COOLDOWN_MS))
    {
        if (_pidOutput > 50.0)
        {
            uint32_t duration = constrain(static_cast<uint32_t>(_pidOutput), PUMP_MIN_DURATION_MS, PUMP_MAX_DURATION_MS);
            startPump(true, duration);      // remplir
        }
        else if (_pidOutput < -50.0)
        {
            uint32_t duration = constrain(static_cast<uint32_t>(-_pidOutput), PUMP_MIN_DURATION_MS, PUMP_MAX_DURATION_MS);
            startPump(false, duration);     // vider
        }
    }
}

void Ballast::setDepthOrder(float targetDepth)
{
    _targetDepth = static_cast<double>(targetDepth);
}

void Ballast::setTunings(float Kp, float Ki, float Kd)
{
    _pid.SetTunings(Kp, Ki, Kd);
}

void Ballast::emergencyStop()
{
    stopPump();
    _pid.SetMode(MANUAL);
    _pidOutput = 0.0;
}

float Ballast::getOutput() const
{
    return static_cast<float>(_pidOutput);
}

bool Ballast::isPumpActive() const
{
    return _pumpActive;
}

void Ballast::emptying(bool startFlag)
{
    if(startFlag)
    {
        digitalWrite(PUMP_FILL_PIN,  LOW);
        digitalWrite(PUMP_EMPTY_PIN, HIGH);
    }
    else
    {
        digitalWrite(PUMP_FILL_PIN,  LOW);
        digitalWrite(PUMP_EMPTY_PIN, LOW);
    }
}

void Ballast::startPump(bool fill, uint32_t duration_ms)
{
    _pumpActive     = true;
    _pumpFilling    = fill;
    _pumpStartTime  = millis();
    _pumpDuration   = duration_ms;
    _lastPumpAction = millis();

    if (fill) 
    {
        digitalWrite(PUMP_FILL_PIN,  HIGH);
        digitalWrite(PUMP_EMPTY_PIN, LOW);
    } else 
    {
        digitalWrite(PUMP_FILL_PIN,  LOW);
        digitalWrite(PUMP_EMPTY_PIN, HIGH);
    }
}

void Ballast::stopPump()
{
    digitalWrite(PUMP_FILL_PIN,  LOW);
    digitalWrite(PUMP_EMPTY_PIN, LOW);
    _pumpActive = false;
}