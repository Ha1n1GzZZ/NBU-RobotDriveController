#ifndef _STEPPER_MOTOR_H_
#define _STEPPER_MOTOR_H_
#include "main.h"
#include "math.h"
#define Pulley_radius 18.5
//#define circumference 18.526  //���ܳ�,��λmm,ע����鶼��һ����
//#define circle_change 1.37  //0.25*2*3.14*0.66 0.25Ϊ�߾���0.66����ϵ��
#define K_correct	0.86 //�ۼ�������ϵ��(�����ԴΪ��Ȧ�����Լ���������Ȧ�����ƶ�)
void StepperMotor_SetPosition(int32_t pos);
void trans_direction_to_pulses(int32_t distance);
void StepperMotor_test(int32_t num);

#endif
