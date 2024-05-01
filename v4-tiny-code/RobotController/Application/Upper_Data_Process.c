#include "Upper_Data_Process.h"
#include "string.h"
#include "chassis_move.h"
#include "Stepper_Motor.h"
#include "ArmSolution.h"
#include "stdio.h"

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
						chassis_move(50,0.06,0);
				else if(direction==1) //go forward&left
					chassis_move(50,3.146,0);
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
			printf("ok\n");
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