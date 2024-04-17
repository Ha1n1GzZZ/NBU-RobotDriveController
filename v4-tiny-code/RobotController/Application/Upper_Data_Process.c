#include "Upper_Data_Process.h"
#include "string.h"
#include "chassis_move.h"
#include "Stepper_Motor.h"
#include "ArmSolution.h"

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
			break;
		}
		case 'R': //Rotate
		{
			float angle=ValidData[3]*100+ValidData[4]*10+ValidData[5];
			chassis_rotate(angle);
			break;
		}
		case 'C': //Correct
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
		case 'G': //Arm移动
		{			
			int8_t Grab_floor=ValidData[2];
			if(Grab_floor==1)
			{
				SetServoAngle(0,114);			//机械臂抓取
				SetServoAngle(1,43);
				SetServoAngle(2,250); //catch
			}
			else if(Grab_floor==2)
			{
				SetServoAngle(0,76);			//机械臂抓取
				SetServoAngle(1,85);
				SetServoAngle(2,250); //catch
			}
			else if(Grab_floor==3)
			{
				SetServoAngle(0,94);			//机械臂抓取
				SetServoAngle(1,88);
				SetServoAngle(2,250); //catch
			}
			break;
		}
		case 'D':  //detect
		{
			uint8_t floor=ValidData[2];
			if(floor==0)                 //detect the lists
			{
				StepperMotor_SetPosition(370);
				SetServoAngle(0,80);
				SetServoAngle(1,135);
				SetServoAngle(2,89);
			}
			else if(floor==1)
			{
				StepperMotor_SetPosition(5);
				SetServoAngle(0,114);
				SetServoAngle(1,116);
				SetServoAngle(2,89);
			}
			else if(floor==2)
			{
				StepperMotor_SetPosition(325);
				SetServoAngle(0,76);
				SetServoAngle(1,164);
				SetServoAngle(2,89);
			}
			else if(floor==3)
			{
				StepperMotor_SetPosition(415);
				SetServoAngle(0,94);			//机械臂抓取
				SetServoAngle(1,165);
				SetServoAngle(2,89); //catch
			}
		}
	}
		
}