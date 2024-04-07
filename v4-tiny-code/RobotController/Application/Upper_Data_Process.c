#include "Upper_Data_Process.h"
#include "string.h"
#include "chassis_move.h"
#include "Stepper_Motor.h"
int16_t ValidData[15];
void char_to_int(char *data)
{
	uint8_t temp[15]={0};
	ValidData[1]=data[1];
	for(uint8_t i=2;i<15;i++)
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
			float speed=ValidData[6]*1000+ValidData[7]*100+ValidData[8]*10+ValidData[9];
			chassis_move(100,target_theta,0);
			break;
		}
		case 'R': //Rotate
		{
			float angle=ValidData[3]*100+ValidData[4]*10+ValidData[5];
			chassis_rotate(angle);
			break;
		}
		case 'S': //Stop
		{
			chassis_move(0,0,0);
			break;
		}
		case 'A': //Arm移动
		{
			int32_t distance=ValidData[2]*1000+ValidData[3]*100+ValidData[4]*10+ValidData[5];
			StepperMotor_SetPosition(distance);
			int8_t Grab_floor=ValidData[6];
			if(Grab_floor==1)
			{
							//机械臂抓取
			}
			else if(Grab_floor==2)
			{
				
			}
			else if(Grab_floor==3)
			{
				
			}
			break;
		}
	}
		
}