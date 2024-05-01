#ifndef _STEPPER_MOTOR_H_
#define _STEPPER_MOTOR_H_
#include "main.h"
#include "math.h"
#define Pulley_radius 18.5
//#define circumference 18.526  //轴周长,单位mm,注意各组都不一样！
//#define circle_change 1.37  //0.25*2*3.14*0.66 0.25为线径，0.66矫正系数
#define K_correct	0.86 //累计误差矫正系数(误差来源为线圈缠绕以及滑轮上线圈左右移动)
void StepperMotor_SetPosition(int32_t pos);
void trans_direction_to_pulses(int32_t distance);
void StepperMotor_test(int32_t num);

#endif
