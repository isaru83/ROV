#include "MotionControlUnit.h"
#include "Pins.h"

MotionControlUnit::MotionControlUnit():_imu(-1),_yaw(YAW_PID_KP,YAW_PID_KI,YAW_PID_KD,&_yawOrder,&_curAngle.yaw)
                                               ,_pitch(PITCH_PID_KP,PITCH_PID_KI,PITCH_PID_KD,&_pitchOrder,&_curAngle.pitch)
                                               ,_yawOrder(0.0),_pitchOrder(0.0),_hThrottle(0),_vThrottle(0),_curAngle({0.0,0.0,0.0})
                                               ,_esc1(PIN_ESC1),_esc2(PIN_ESC2)
                                               ,_esc3(PIN_ESC3),_esc4(PIN_ESC4)
                                               ,_started(false),_fluidDensity(LAKE_FLUID_DENSITY)
                                               ,_telemetryTimer(TELEMETRY_UPDATE_TIME)
{
  _telemetry = {0};
}

uint8_t MotionControlUnit::init()
{
  int err = _imu.init();
  if(err != ERR_OK)
    return err;
  delay(500);
  if(! _pressure.init())    
    return ERR_PRESS_SENSOR;
  _pressure.setFluidDensity(_fluidDensity);

  if (!_esc1.begin() || !_esc2.begin() || !_esc3.begin() || !_esc4.begin()) 
        return ERR_ESC_INIT;
  _ballast.begin();
  
  return ERR_OK;
}

void MotionControlUnit::setOrder(float yawOrder,float pitchOrder, uint16_t hTrottle, uint16_t vThrottle)
{
  _yawOrder = (double)yawOrder;
  _pitchOrder = (double)pitchOrder;
  _hThrottle = hTrottle;
  _vThrottle = vThrottle;
}

void MotionControlUnit::setBallastOrder(float ballastOrder)
{
  _ballast.setDepthOrder(ballastOrder);
}

void MotionControlUnit::update()
{
  if(_started)
  {
    if(_imu.update())
    {
      _curAngle = *_imu.getAngle();
      double yawPidCorrection = _yaw.compute();
      double pitchPidCorrection = _pitch.compute();
      _esc1.write(constrain(_vThrottle + pitchPidCorrection + ESC_NEUTRAL, ESC_MIN, ESC_MAX));
      _esc2.write(constrain(_vThrottle - pitchPidCorrection + ESC_NEUTRAL, ESC_MIN, ESC_MAX));
      _esc3.write(constrain(_hThrottle + yawPidCorrection + ESC_NEUTRAL, ESC_MIN, ESC_MAX));
      _esc4.write(constrain(_hThrottle - yawPidCorrection + ESC_NEUTRAL, ESC_MIN, ESC_MAX));
    }
    _ballast.update(_telemetry.depth);
  }

  if(_telemetryTimer.isTrigged())
  {
    _pressure.read();
    _telemetry.depth = _pressure.depth();
    _telemetry.temperature = _pressure.temperature();
    _telemetry.pressure = _pressure.pressure();
    _telemetry.angle = *_imu.getAngle_F();
    _telemetry.accel = *_imu.getAccel();
    _telemetry.heading = _imu.getTiltCompensatedHeading();
  }
}

void MotionControlUnit::escArm()
{
  //Calibration ESCs
  _esc1.write(ESC_MIN_US);
  _esc2.write(ESC_MIN_US);
  _esc3.write(ESC_MIN_US);
  _esc4.write(ESC_MIN_US);  
  delay(1000);
  _esc1.write(ESC_MAX_US);
  _esc2.write(ESC_MAX_US);
  _esc3.write(ESC_MAX_US);
  _esc4.write(ESC_MAX_US);
  delay(1000); 
  _esc1.write(ESC_NEUTRAL_US);
  _esc2.write(ESC_NEUTRAL_US);
  _esc3.write(ESC_NEUTRAL_US);
  _esc4.write(ESC_NEUTRAL_US);
  delay(2000);
}

void MotionControlUnit::escStop()
{
  _esc1.disarm(); 
  _esc2.disarm(); 
  _esc3.disarm(); 
  _esc4.disarm();
}

void MotionControlUnit::start()
{
  _started = true;
  _ballast.emptying(true);
  escArm();              // 4 secondes
  delay(2000);          // + 2 secondes, 6 secondes en tout de vidange 
  _ballast.emptying(false);
  _ballast.start(); 
  _imu.update();
  _pitchOrder = *_imu.getYaw();
}

void MotionControlUnit::stop()
{
  _started = false;
  escStop();
}

MCU_Telemetry *MotionControlUnit::getTelemetry()
{
  return &_telemetry;
}

