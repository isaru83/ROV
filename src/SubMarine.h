#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "MotionControlUnit.h"
#include "SerialManager.h"
#include "pins.h"
#include <Arduino.h>

#define PITCH 0
#define YAW   1

#define FAILSAFE_TIMEOUT_MS  1200

class SubMarine
{
    public:
        SubMarine();
        ~SubMarine();
        uint8_t begin();
        void run();
        void emergencyStop();
        MCU_Telemetry *getTelemetry();
        bool isRunning() const;
        void enterFailsafe();
        void exitFailsafe();

    private:
        void processIncomingCommands();
        void processPilotCommand(const uint8_t* data, uint8_t len);
        void processGetPidTunings(const uint8_t* data, uint8_t len);
        void processSetPidTunings(const uint8_t* data, uint8_t len);
        void sendTelemetry();

        SerialManager      _serial;
        MotionControlUnit  _mcu;
        bool               _running = false;
        bool               _inFailsafe = false;
        uint32_t           _lastCommandTime = 0;
};

#endif