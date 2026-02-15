#include "IMU.h"

IMU::IMU(int rstFlag):_imu(rstFlag),_curAngle({0.0,0.0,0.0}),_curAngleFloat({0.0,0.0,0.0}),_accel({0.0,0.0,0.0}),_angleAccel({0.0,0.0,0.0})
                      ,_velocity({0.0,0.0,0.0}),_prevAcc({0.0,0.0,0.0}),_updateTime(IMU_UPDATE_TIME/1000.0)
{
  
}

uint8_t IMU::init()
{
  if(!_imu.begin_I2C(BNO085_ADDR))
    return ERR_IMU_I2C;
  delay(500);
  if(!_imu.enableReport(SH2_LINEAR_ACCELERATION))
    return ERR_IMU_REPORT_ACC;
  if(!_imu.enableReport(SH2_ROTATION_VECTOR))
    return ERR_IMU_REPORT_ANGL;
  if(!_imu.enableReport(SH2_GYROSCOPE_CALIBRATED))
    return ERR_IMU_REPORT_GYRO;
  if (!_imu.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED))
    return ERR_IMU_REPORT_MAG;

  update();
  return ERR_OK;
}

bool IMU::update()
{
  if(_imu.getSensorEvent(&_sensorValue))
  {
    switch(_sensorValue.sensorId)
    {
      case SH2_GYROSCOPE_CALIBRATED:
      {
        _angleAccel.x = _sensorValue.un.gyroscope.x;
        _angleAccel.y = _sensorValue.un.gyroscope.y;
        _angleAccel.z = _sensorValue.un.gyroscope.z;
        break;
      }
      case SH2_ROTATION_VECTOR:
      {
          quaternionToEuler(_sensorValue.un.rotationVector.real,_sensorValue.un.rotationVector.i
                            ,_sensorValue.un.rotationVector.j,_sensorValue.un.rotationVector.k,&_curAngle);
          convertAngleDoubleToFloat();
          break;
      }
      case SH2_LINEAR_ACCELERATION:
      {
        _accel.x = (float)_sensorValue.un.linearAcceleration.x;
        _accel.y = (float)_sensorValue.un.linearAcceleration.y;
        _accel.z = (float)_sensorValue.un.linearAcceleration.z;
        _velocity.x += (_accel.x + _prevAcc.x) / 2.0 * _updateTime;
        _velocity.y += (_accel.y + _prevAcc.y) / 2.0 * _updateTime;
        _velocity.z += (_accel.z + _prevAcc.z) / 2.0 * _updateTime;
        _prevAcc = _accel;
        break;
      }
      case SH2_MAGNETIC_FIELD_CALIBRATED:
      {
        // Valeurs en micro-Tesla (µT)
        float magX = _sensorValue.un.magneticField.x;
        float magY = _sensorValue.un.magneticField.y;
        float magZ = _sensorValue.un.magneticField.z;

        _mag.x = magX;
        _mag.y = magY;
        _mag.z = magZ;
        break;
      }
      default:
        break;
    }
    return true;
  }
  return false;
}

float IMU::getTiltCompensatedHeading() const
{
    // On a besoin du pitch et roll (depuis rotation vector ou accéléro)
    float pitch = _curAngle.pitch;   // en degrés
    float roll  = _curAngle.roll;

    float pitchRad = pitch * PI / 180.0;
    float rollRad  = roll  * PI / 180.0;

    // Projection du champ magnétique dans le plan horizontal
    float magX_h = _mag.x * cos(pitchRad) + _mag.z * sin(pitchRad);
    float magY_h = _mag.x * sin(rollRad)  * sin(pitchRad) +
                   _mag.y * cos(rollRad)  -
                   _mag.z * sin(rollRad)  * cos(pitchRad);

    float heading = atan2(magY_h, magX_h) * 180.0 / PI;
    if (heading < 0) heading += 360.0;

    return heading;
}

void IMU::quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr)
{
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr)) * RAD_TO_DEG;
    ypr->roll = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr)) * RAD_TO_DEG;
    ypr->pitch = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr)) * RAD_TO_DEG;
}

void IMU::convertAngleDoubleToFloat() {
    _curAngle.roll  = (float)(_curAngle.roll);
    _curAngle.pitch = (float)(_curAngle.pitch);
    _curAngle.yaw   = (float)(_curAngle.yaw);
}