#ifndef MOTIONCONTROLUNIT_H_INCLUDED
#define MOTIONCONTROLUNIT_H_INCLUDED

#include "ESC.h"
#include <Wire.h>
#include <MS5837.h>
#include "IMU.h"
#include "AxeManagement.h"
#include "Error.h"
#include "MyTimer.h"
#include "Ballast.h"

#define YAW_PID_KP 1.0
#define YAW_PID_KI 1.0
#define YAW_PID_KD 1.0

#define PITCH_PID_KP 1.0
#define PITCH_PID_KI 1.0
#define PITCH_PID_KD 1.0

#define LAKE_FLUID_DENSITY  997
#define SEA_FLUID_DENSITY  1025

#define TELEMETRY_UPDATE_TIME 500  //ms

struct MCU_Telemetry 
{
  euler_t_F angle;
  Vector3 accel;
  float heading;
  float temperature;
  float depth;
  float pressure;
}__attribute__((packed));

class MotionControlUnit
{
  friend class SubMarine;

  public:
    MotionControlUnit();
    uint8_t init();
    void setOrder(float yawOrder,float pitchOrder, uint16_t hTrottle, uint16_t vThrottle);
    void setBallastOrder(float ballastOrder);
    MCU_Telemetry *getTelemetry();
    
    void start();
    void stop();
    void update();

  private:
    void escArm();
    void escStop();

    IMU _imu;
    AxeManagement _yaw,_pitch;
    double _yawOrder,_pitchOrder;
    uint16_t _hThrottle,_vThrottle;
    euler_t _curAngle;
    ESC _esc1,_esc2,_esc3,_esc4;
    bool _started;
    uint16_t _fluidDensity;
    Timer _telemetryTimer;
    MS5837 _pressure;
    MCU_Telemetry _telemetry;
    Ballast _ballast;
};

#endif // TIMER_H_INCLUDED
