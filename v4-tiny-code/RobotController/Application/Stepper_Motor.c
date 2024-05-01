#include "Stepper_Motor.h"
#include "stdlib.h"
#include "delay.h"

//步进电机控制程序，步距角1.8°，16细分，3200个脉冲转一圈

/* 外设接口定义：控制IO电平 */
#define Change_Stepper_Dir HAL_GPIO_TogglePin(Stepper_Dir_GPIO_Port,Stepper_Dir_Pin);
#define Reversal_Stepper HAL_GPIO_TogglePin(Stepper_Dir_GPIO_Port,Stepper_Stp_Pin);

int8_t direction = 1; // 1:正转 -1:反转
int32_t last_position = 0; // 上一次的位置
int32_t position=0;
int64_t pulsenum=0;

void trans_direction_to_pulses(int32_t distance) {
    pulsenum = 0;  
		float temp=(abs(distance)*3200/Pulley_radius);                   //此处用于计算移动距离与脉冲之间的关系
    pulsenum =  (int)temp;
}

void StepperMotor_SetPosition(int32_t pos) { //传入需要到达的位置,单位mm
    // Send pulses to the Stepper_Stp port to drive the stepper motor
		position=pos;
    int32_t distance = position - last_position;
		last_position = pos;
    trans_direction_to_pulses(distance);
    if(distance*direction>0)
    {	
        for(int64_t i=0;i<pulsenum;i++)
        {
            Reversal_Stepper;
            delay_us(70);   //该步进电机最大空载启动频率为1500pps,最小延时时间为666us,但是实操目前尚未发现问题
						Reversal_Stepper;   //相当于一个脉冲
        }
				
    }
    else
    {
        Change_Stepper_Dir;
        direction=-direction;
        for(int64_t i=0;i<pulsenum;i++)
        {
            Reversal_Stepper;
						delay_us(70);
            Reversal_Stepper;
        }
    }
}