#ifndef __CHASSIS_MOVE_H_
#define __CHASSIS_MOVE_H_

#include "stm32f4xx.h"

typedef struct
{
    int16_t Radius;       // �����뾶
    double angle;       // �����Ƕ�
		
} Chassis;

extern Chassis chassis;
void chassis_move(float target_speed, float target_theta, float target_omega);
void chassis_rotate(float rotation_angle)	;

#endif