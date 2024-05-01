#ifndef _ArmSolution_H_
#define _ArmSolution_H_
#include "main.h"

void ArmDriver_Init(void);
void SetServoAngle(int nServo,float angle);
uint8_t ServoTunnerOK(void);
void Servo_init(uint8_t nServo,int angle);

void ArmSolution(double x,double y);
void Arm_Grab(uint8_t floor);
void Arm_Detect(uint8_t floor);	
void Arm_Put(void);
void slowPwm(uint8_t nServo);

void State2State(int*,double);
#endif


