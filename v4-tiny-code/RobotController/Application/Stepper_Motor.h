#ifndef _STEPPER_MOTOR_H_
#define _STEPPER_MOTOR_H_
#include "main.h"

#define Pulley_radius 15.48  //D�����ܳ�,��λmm
#define K_correct	0.85 //�ۼ�������ϵ��(�����ԴΪ��Ȧ�����Լ���������Ȧ�����ƶ�)
void StepperMotor_SetPosition(int32_t distance);
void trans_direction_to_pulses(int32_t distance);
void StepperMotor_test(int32_t num);
#endif
