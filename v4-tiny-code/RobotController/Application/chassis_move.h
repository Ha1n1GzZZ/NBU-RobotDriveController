#ifndef __CHASSIS_MOVE_H_
#define __CHASSIS_MOVE_H_

#include "stm32f4xx.h"

typedef struct
{
    int16_t Radius;       // ³µÁ¾°ë¾¶
    double angle;       // ³µÁ¾½Ç¶È
		
} Chassis;

extern Chassis chassis;
void chassis_move(float target_speed, float target_theta, float target_omega);
void chassis_rotate(float rotation_angle)	;

#endif