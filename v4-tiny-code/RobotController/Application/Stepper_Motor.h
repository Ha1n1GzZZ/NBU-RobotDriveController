#ifndef _STEPPER_MOTOR_H_
#define _STEPPER_MOTOR_H_
#include "main.h"

#define Pulley_radius 15.48  //D字轴周长,单位mm
#define K_correct	0.85 //累计误差矫正系数(误差来源为线圈缠绕以及滑轮上线圈左右移动)
void StepperMotor_SetPosition(int32_t distance);
void trans_direction_to_pulses(int32_t distance);
void StepperMotor_test(int32_t num);
#endif
