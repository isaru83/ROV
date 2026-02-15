#ifndef IMU_H_INCLUDED
#define IMU_H_INCLUDED

#include <Adafruit_BNO08x.h>
#include <math.h>
#include "MyTimer.h"
#include "Error.h"

#define BNO085_ADDR 0x4A

#define IMU_UPDATE_TIME 20

typedef struct
{
  double yaw, pitch, roll;
}euler_t;

typedef struct
{
  float yaw, pitch, roll;
}euler_t_F;

typedef struct 
{
  float x,y,z;
}Vector3;

class IMU
{
  public:
    IMU(int rstFlag);
    uint8_t init();
    bool update();
    float getTiltCompensatedHeading() const;
    euler_t *getAngle() { return &_curAngle; }
    Vector3 *getAngleAccel() { return &_angleAccel; }
    euler_t_F *getAngle_F() { return &_curAngleFloat; }
    Vector3 *getAccel() { return &_accel;}
    float *getYaw() { return &_curAngleFloat.yaw;}
    float *getPitch() { return &_curAngleFloat.pitch;}
    float *getRoll() { return &_curAngleFloat.roll;}
    float *getXAccel() { return &_accel.x;}
    float *getYAccel() { return &_accel.y;}
    float *getZAccel() { return &_accel.z;}
    
  private:
    void convertAngleDoubleToFloat();
    void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr);

    Adafruit_BNO08x  _imu;
    euler_t _curAngle;
    euler_t_F _curAngleFloat;
    Vector3 _accel, _angleAccel;
    Vector3 _velocity,_prevAcc;
    float _updateTime;
    sh2_SensorValue_t _sensorValue;
    Vector3 _mag;
};

#endif // TIMER_H_INCLUDED
