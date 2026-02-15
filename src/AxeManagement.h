#ifndef AXEMANAGEMENT_H_INCLUDED
#define AXEMANAGEMENT_H_INCLUDED

#include <PID_v1.h>
#include <Arduino.h>
#include "Error.h"

#define PID_MIN  -500 
#define PID_MAX   500 

typedef struct
{
  double kp,ki,kd;
}PidRatio;

class AxeManagement
{
  public:
    AxeManagement(PidRatio ratio,double *setPoint,double *curAngle);
    AxeManagement(double kp,double ki,double kd,double *setPoint,double *curAngle);
    void setKp(double kp);
    void setKi(double ki);
    void setKd(double kd);
    void getPidTunings(float *kp, float *ki, float *kd);
    void setPidTunings(float kp, float ki, float kd);
    double compute(); 

  private:
    PidRatio _pidRatio;
    double *_setPoint, *_curAngle;
    double _output;
    PID _pid;
};

#endif // TIMER_H_INCLUDED
