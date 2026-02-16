#include "SubMarine.h"

SubMarine::SubMarine(): _serial(&Serial2)
{
    
}

uint8_t SubMarine::begin()
{
    uint8_t err = _mcu.init();
    if (err != ERR_OK) return err;

    // Initialisation RS485
    pinMode(PIN_RS485_DE, OUTPUT);
    digitalWrite(PIN_RS485_DE, LOW);
    _lastCommandTime = millis();
    _running = false;
    return ERR_OK;
}

void SubMarine::emergencyStop()
{
    _mcu.stop();
    _running = false;
}

void SubMarine::run()
{
    _serial.update();
    processIncomingCommands();

    if (_running)
    {
        _mcu.update();
        // === FAILSAFE CHECK ===
        if (millis() - _lastCommandTime > FAILSAFE_TIMEOUT_MS)
        {
            if (!_inFailsafe)
                enterFailsafe();
        }
        else if (_inFailsafe)
        {
            exitFailsafe();
        }
    }
}

MCU_Telemetry* SubMarine::getTelemetry()
{
    return _mcu.getTelemetry();
}

bool SubMarine::isRunning() const
{
    return _running;
}

void SubMarine::processIncomingCommands()
{
    if (!_serial.available()) return;

    uint8_t type, id, data[REQ_MAX_SIZE];
    uint8_t len = _serial.getFrame(&type, &id, data);
    if(len == 0)
        return;

    if (type == 0x01) // commande standard
    {  
        processPilotCommand(data,len);
        _lastCommandTime = millis();
    }
    else if(type == 0x02) // Demande telemetrie
    {
        sendTelemetry();
        _lastCommandTime = millis();
    }
    else if(type == 0x03) // Demande Coef PID
    {
        processGetPidTunings(data, len);
        _lastCommandTime = millis();
    }
    else if(type == 0x04) // Modif Coef PID
    {
        processSetPidTunings(data, len);
        _lastCommandTime = millis();
    }
    else if(type == 0x05) // Modif densite fluid
    {
        if(len < 2) return;
        uint16_t density = (data[0] << 8) | data[1];
        _mcu._pressure.setFluidDensity(density);
        _lastCommandTime = millis();
    }
    else if(type == 0x08) // Demarage
    {
        _running = true;
        _mcu.start();
        _lastCommandTime = millis();
    }
}

void SubMarine::processSetPidTunings(const uint8_t* data, uint8_t len)
{
    if(len != 13) return;

    float kp,ki,kd;
    uint8_t axeFlag = data[0];

    memcpy(&kp, &data[1], sizeof(float));
    memcpy(&ki, &data[5], sizeof(float));
    memcpy(&kd, &data[9], sizeof(float));

    if (isnan(kp) || isnan(ki) || isnan(kd))  return;
    
    if(axeFlag == PITCH)
        _mcu._pitch.setPidTunings(kp,ki,kd);
    else if(axeFlag == YAW)
        _mcu._yaw.setPidTunings(kp,ki,kd);
}

void SubMarine::processGetPidTunings(const uint8_t* data, uint8_t len)
{
    if (len < 1) return;

    uint8_t axeFlag = data[0];
    float kp = 0, ki = 0, kd = 0;

    if (axeFlag == PITCH)
        _mcu._pitch.getPidTunings(&kp, &ki, &kd);
    else if (axeFlag == YAW)
        _mcu._yaw.getPidTunings(&kp, &ki, &kd);
    else 
        return;

    uint8_t buf[13];
    buf[0] = axeFlag; 
    memcpy(&buf[1], &kp, 4);
    memcpy(&buf[5], &ki, 4);
    memcpy(&buf[9], &kd, 4);

    _serial.sendFrame(0x81, buf, 13);  
}

void SubMarine::processPilotCommand(const uint8_t* data, uint8_t len)
{
    if (len < 14) return; 

    int16_t vThrottle = (data[0] << 8) | data[1];
    int16_t hThrottle = (data[2] << 8) | data[3];

    float depthOrder, pitchOrder, yawOrder;
    memcpy(&depthOrder, &data[4], 4);
    memcpy(&pitchOrder, &data[8], 4);
    memcpy(&yawOrder, &data[12], 4);

    vThrottle = constrain(vThrottle, -500, 500);
    hThrottle = constrain(hThrottle, -500, 500);

    if (isnan(depthOrder) || isnan(pitchOrder) || isnan(yawOrder))
        return;

    _mcu.setOrder(yawOrder, pitchOrder, hThrottle, vThrottle);
    _mcu.setBallastOrder(depthOrder);
}

void SubMarine::sendTelemetry()
{
    const MCU_Telemetry t = *_mcu.getTelemetry();
    uint8_t buf[64];
    uint8_t len = 0;

    memcpy(&buf[len], &t.angle.pitch, 4); len += 4;
    memcpy(&buf[len], &t.angle.roll,  4); len += 4;
    memcpy(&buf[len], &t.angle.yaw,   4); len += 4;
    memcpy(&buf[len], &t.accel.x,     4); len += 4;
    memcpy(&buf[len], &t.accel.y,     4); len += 4;
    memcpy(&buf[len], &t.accel.z,     4); len += 4;
    memcpy(&buf[len], &t.heading,     4); len += 4;
    memcpy(&buf[len], &t.temperature, 4); len += 4;
    memcpy(&buf[len], &t.depth,       4); len += 4;
    memcpy(&buf[len], &t.pressure,    4); len += 4;

    _serial.sendFrame(0x80, buf, len);    
}

// ────────────────────────────────────────────────
// Failsafe
// ────────────────────────────────────────────────

void SubMarine::enterFailsafe()
{
    _inFailsafe = true;
    // 1. Vider le ballast complètement
    _mcu.setBallastOrder(0.0f);           // ou appeler directement emptyBallast() si tu as une fonction
    // 2. Très léger hThrottle (avance lente)
    // 3. Pitch à 0.0 (assiette horizontale)
    _mcu.setOrder(0.0f, 0.0f, 1620, 1500);   // hThrottle = 1620 (très léger), vThrottle = neutre
}

void SubMarine::exitFailsafe()
{
    if (!_inFailsafe) return;

    _inFailsafe = false;
    // On remet les throttles à 0 par sécurité
    _mcu.setOrder(0.0f, 0.0f, 1500, 1500);
}