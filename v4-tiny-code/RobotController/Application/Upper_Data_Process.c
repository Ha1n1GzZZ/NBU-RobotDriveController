#include "Upper_Data_Process.h"
#include "string.h"
#include "chassis_move.h"
#include "Stepper_Motor.h"
#include "ArmSolution.h"
#include "stdio.h"
#include "motor_controller.h"
uint8_t Is_cube=0;
int16_t ValidData[12];
void char_to_int(char *data)
{
	uint8_t temp[12]={0};
	ValidData[1]=data[1];
	for(uint8_t i=2;i<12;i++)
	{
		ValidData[i]=data[i]-'0';
	}
}

void data_process(char *data)
{
	char_to_int(data); //将字符转换成整型
	switch (ValidData[1])
	{
		case 'M':  //Move
		{
			float target_theta=ValidData[2]+ValidData[4]*0.1+ValidData[5]*0.01; //ValidData[3] is point
			float speed=ValidData[6]*100+ValidData[7]*10+ValidData[8];
			uint8_t Is_detecting=ValidData[9];
			uint8_t direction=ValidData[10];
			if(Is_detecting==0)
				chassis_move(speed,target_theta,0);
			else if(Is_detecting==1)
			{
				if(direction==0) //go forward&right
						chassis_move(50,0.07,0);
				else if(direction==1) //go forward&left
					chassis_move(50,3.17,0);
			}
			printf("ok\n");
			break;
		}
		case 'R': //Rotate
		{
			float angle=ValidData[3]*100+ValidData[4]*10+ValidData[5];
			chassis_rotate(angle);
			printf("ok\n");
			break;
		}
		case 'C': //Correct
		{
			chassis_move(100,1.57,0);
			HAL_Delay(1);
			break;
		}
		case 'J': //little correct when focus on goods
		{
			chassis_move(100,1.57,0);
			HAL_Delay(1);
			break;
		}
		case 'S': //Stop
		{
			chassis_move(0,0,0);
			break;
		}
		case 'G': //Arm_Grab
		{			
			uint8_t Grab_floor=ValidData[2];
			Is_cube=ValidData[3];
			Arm_Grab(Grab_floor);
			printf("ok\n");
			break;
		}
		case 'D':  //detect
		{
			uint8_t floor=ValidData[2];				
			Arm_Detect(floor);
			printf("ok\n");
			break;
		}
		case 'N': //theta correct
		{
			float Cur_theta=ValidData[3]*10+ValidData[4];
			if(ValidData[2]=='-'-'0')
			{
				Cur_theta=-Cur_theta;
			}
			if(Cur_theta>0)
			{   
				MotorController_SetSpeed(1,-50);		   //forward&left circle
				MotorController_SetSpeed(2,-50);
				MotorController_SetSpeed(3,-50);
				MotorController_SetSpeed(4,-50);
				HAL_Delay((int)Cur_theta*50);
				chassis_move(0,0,0);
				HAL_Delay(1);
				chassis_move(100,1.57,0);
				HAL_Delay(600);
				chassis_move(0,0,0);
				printf("ok\n");
			}
			else if(Cur_theta<0)
			{
				MotorController_SetSpeed(1,50);		
				MotorController_SetSpeed(2,50);
				MotorController_SetSpeed(3,50);
				MotorController_SetSpeed(4,50);
				HAL_Delay((int)(fabs(Cur_theta))*50);
				chassis_move(0,0,0);
				HAL_Delay(1);
				chassis_move(100,1.57,0);
				HAL_Delay(600);
				chassis_move(0,0,0);
				printf("ok\n");
			}
			break;
		}
		case 'P': //Put
		{
			Arm_Put();
			printf("ok\n");
			break;
		}
		case 'H': //Hou Tui
		{
			chassis_move(100,4.71,0);
			printf("ok\n");
			break;
		}
	}
		
}