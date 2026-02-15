#include "AxeManagement.h"

AxeManagement::AxeManagement(PidRatio ratio,double *setPoint,double *curAngle):_pidRatio(ratio), _setPoint(setPoint)
                                                                                              ,_curAngle(curAngle),_output(0.0)
                                                                                              ,_pid(curAngle,&_output,setPoint,_pidRatio.kp,_pidRatio.ki,_pidRatio.kd,DIRECT)
{
  _pid.SetMode(AUTOMATIC);
  _pid.SetOutputLimits(PID_MIN,PID_MAX);
}

AxeManagement::AxeManagement(double kp,double ki,double kd,double *setPoint,double *curAngle):_pidRatio({kp,ki,kd}), _setPoint(setPoint)
                                                                                              ,_curAngle(curAngle),_output(0.0)
                                                                                              ,_pid(curAngle,&_output,setPoint,_pidRatio.kp,_pidRatio.ki,_pidRatio.kd,DIRECT)
{
  _pid.SetMode(AUTOMATIC);
  _pid.SetOutputLimits(PID_MIN,PID_MAX);
}


double AxeManagement::compute()
{
  _pid.Compute();
  return _output;
}

void AxeManagement::getPidTunings(float *kp, float *ki, float *kd)
{
    *kp = static_cast<float>(_pidRatio.kp);
    *ki = static_cast<float>(_pidRatio.ki);
    *kd = static_cast<float>(_pidRatio.kd);
}
void AxeManagement::setPidTunings(float kp, float ki, float kd)
{
    _pidRatio.kp = kp;
    _pidRatio.ki = ki;
    _pidRatio.kd = kd;
}

void AxeManagement::setKp(double kp)
{
  _pid.SetTunings(kp,_pidRatio.ki,_pidRatio.kd);
  _pidRatio.kp = kp;
}

void AxeManagement::setKi(double ki)
{
  _pid.SetTunings(_pidRatio.kp,ki,_pidRatio.kd);
  _pidRatio.ki = ki;
}

void AxeManagement::setKd(double kd)
{
  _pid.SetTunings(_pidRatio.kp,_pidRatio.ki,kd);
  _pidRatio.kd = kd;
}
