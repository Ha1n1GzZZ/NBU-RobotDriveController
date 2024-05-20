#include "math.h"
#include "chassis_move.h"
#include "motor_controller.h"
#include "motor_driver.h"
#include "mpu6500dmp.h"
//使用本程序时切记如果底盘安装方式不同会导致解算出的值不同！！！！
Chassis chassis;
/*  全局坐标系与车辆初始坐标系相同
		0———————————————————————>X
		1
		1
		1
		1
		1
		Y
*/
void chassis_move(float target_speed, float target_theta, float target_omega) //target_speed: mm/s, target_theta: rad, target_omega: rad/s 角度值由imu提供,
{
		chassis.Radius = 300; //底盘半径
//	  Get_MPU6500_DMP_Data();  //尚未解决mpu数据问题
		chassis.angle=0;//底盘自旋情况下，改变机器人坐标系和全局坐标系的粗略计算
//		chassis.angle=mpu_yaw*3.14/180;
    float speed[4]; //速度计算值
    float sin_ang = sin(chassis.angle);   //全局坐标系下的sin角度
    float cos_ang = cos(chassis.angle);   //全局坐标系下的cos角度
    float speed_X = target_speed * cos(target_theta); //全局坐标系下的x轴速度
    float speed_Y = target_speed * sin(target_theta); //全局坐标系下的y轴速度

    speed[0] = ((-cos_ang - sin_ang) * speed_X + (-sin_ang + cos_ang) * speed_Y + chassis.Radius * target_omega) / sqrt(2);
    speed[1] = ((-cos_ang + sin_ang) * speed_X + (-sin_ang - cos_ang) * speed_Y + chassis.Radius * target_omega) / sqrt(2);
    speed[2] = ((cos_ang + sin_ang) * speed_X + (sin_ang - cos_ang) * speed_Y + chassis.Radius * target_omega) / sqrt(2);
    speed[3] = ((cos_ang - sin_ang) * speed_X + (sin_ang + cos_ang) * speed_Y + chassis.Radius * target_omega) / sqrt(2);
	
    for (int i = 0; i < 4; i++)
		{
     MotorController_SetSpeed(i+1,(int)speed[i]);
    }

}

void chassis_rotate(float rotation_angle)		//rotation_angle：°，目标电机旋转角度
{
	float target_omega=0.5;		//旋转速度
	chassis_move(0,0,target_omega);		//使底盘以角速度旋转

	int32_t target_nCnt =(((rotation_angle / 360.00f) * chassis.Radius * 2 * 3.14f)/(MOTOR_WHEEL_DIAMETER * 2 * 3.14f))* 13500 * 4;//(rotation_angle / 360.000f) * 135000 * 4;	//原公式为：(((rotation_angle / 360.000f) * chassis.Radius * 2 * 3.14f)/(MOTOR_WHEEL_DIAMETER * 2 * 3.14f))* 13500 * 4; 	//计算转够角度编码器增加的脉冲数
	int32_t nCnt0 = Encoder_GetEncCount(1);		// 获取编码器初始计数值
	
	while(1)      //时刻判断是否转够角度
	{
		int32_t nCnt = Encoder_GetEncCount(1);	
		int32_t nCnt1 = nCnt - nCnt0;			//计算电机开始旋转后编码器累计计数值。
		if(nCnt1 >= target_nCnt)		//转够角度则停止转动，并退出循环
		{
			chassis_move(0,0,0);
			break;
		}
	}
	
}

