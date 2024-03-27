#ifndef _STEPPER_MOTOR_H_
#define _STEPPER_MOTOR_H_
#include "main.h"

#define Pulley_radius 10  //»¬ÂÖ°ë¾¶,µ¥Î»mm
void StepperMotor_SetPosition(int32_t distance);
int64_t trans_direction_to_pulses(int32_t distance);
void StepperMotor_test(int32_t num);
#endif
