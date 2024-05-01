
#include "ArmSolution.h"
#include "delay.h"
#include "math.h"
#include "tim.h"
#include "stdlib.h"
#include "Stepper_Motor.h"
#define PWM_DUTY_LIMIT 10000 // PWM占空比周期为10000,代表20ms    250-1250 代表 0-180度

// 每个舵机缓慢移动的目标pwm
uint16_t targetPwm[8] = {250};


int current_arm_state[2] ={80,176};  //机械臂当前状态
int previous_arm_state[2] ={80,176}; //机械臂上次状态

int Arm_Detect_floor0[2] = {80,100}; //
int Arm_Detect_floor1[2] = {122,88}; //
int Arm_Detect_floor2[2] = {104,106};//
int Arm_Detect_floor3[2] = {118,106};//

int Arm_Grab_floor1[2] = {104,30};   //
int Arm_Grab_floor2[2] = {85,62};    //
int Arm_Grab_floor3[2] = {88,68};    //

int Arm_Putdown[2] = {120,160};      //


void ArmDriver_Init(void){
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);

	//直接给pwm赋初值  初始设定为初始机械臂状态  每次上电前应该把机械臂归位
	//下面为使用举例
	Servo_init(0,80);
	Servo_init(1,176);
	Servo_init(2,80);

}
void Servo_init(uint8_t nServo,int angle)
{
	volatile uint32_t *currentPwm;//指针指向不同的pwm控制地址，方便统一修改。
	switch (nServo)
	{
	case 0:
		currentPwm = &htim8.Instance->CCR3;
		break;
	case 1:
		currentPwm = &htim8.Instance->CCR4;
		break;
	case 2:
		currentPwm = &htim12.Instance->CCR1;
		break;
	case 3:
		currentPwm = &htim12.Instance->CCR2;
		break;
	case 4:
		currentPwm = &htim9.Instance->CCR1;
		break;
	case 5:
		currentPwm = &htim9.Instance->CCR2;
		break;
	case 6:
		currentPwm = &htim13.Instance->CCR1;
		break;
	case 7:
		currentPwm = &htim14.Instance->CCR1;
		break;
	default:
		return;
}
	if (angle < 0) // 角度限制
		return;
	int pwm = angle * 1.0f / 180 / 20 * PWM_DUTY_LIMIT + 250; // 解算出对应的pwm波

	// pwm限制，防止动作错误
	if (pwm > 1250)
		pwm = 1250;
	if (pwm < 250)
		pwm = 250;

	*currentPwm = pwm;	//设置定时器CCR中PWM值
	targetPwm[nServo]=pwm;	//设置目标PWM值
}

/**
 * @description: 设置舵机角度
 * @param {int} nServo几号舵机,从0-7
 * @param {float} angle角度(0-180)
 * @return {*}
 */
void SetServoAngle(int nServo, float angle)
{
	if (angle < 0) // 角度限制
		return;
	int pwm = angle * 1.0f / 180 / 20 * PWM_DUTY_LIMIT + 250; // 解算出对应的pwm波

	// pwm限制，防止动作错误
	if (pwm > 1250)
		pwm = 1250;
	if (pwm < 250)
		pwm = 250;

	targetPwm[nServo] = pwm;
}

/* 判断调整是否结束 即每个舵机角度是否都与目标一致 
	需要按照自己的需求去删去一些没用到的舵机pwm判断
*/
uint8_t ServoTunnerOK(void)	
{
	if(targetPwm[0] == htim8.Instance->CCR3&& \
	   targetPwm[1] == htim8.Instance->CCR4&& \
	   targetPwm[2] == htim12.Instance->CCR1)
//	   targetPwm[3] == htim12.Instance->CCR2&& \
//	   targetPwm[4] == htim9.Instance->CCR1&& \
//	   targetPwm[5] == htim9.Instance->CCR2&& \
//	   targetPwm[6] == htim13.Instance->CCR1&& \
//	   targetPwm[7] == htim14.Instance->CCR1) 
//	if (targetPwm[0] >= htim8.Instance->CCR3-10&&targetPwm[0] <= htim8.Instance->CCR3+10) 
		return 1;
	else
		return 0;
}

/*舵机缓慢移动函数，不用看核心要义就是慢慢改变*/
void slowPwm(uint8_t nServo)
{
	volatile uint32_t *currentPwm;//指针指向不同的pwm控制地址，方便统一修改。
	switch (nServo)
	{
	case 0:
		currentPwm = &htim8.Instance->CCR3;
		break;
	case 1:
		currentPwm = &htim8.Instance->CCR4;
		break;
	case 2:
		currentPwm = &htim12.Instance->CCR1;
		break;
	case 3:
		currentPwm = &htim12.Instance->CCR2;
		break;
	case 4:
		currentPwm = &htim9.Instance->CCR1;
		break;
	case 5:
		currentPwm = &htim9.Instance->CCR2;
		break;
	case 6:
		currentPwm = &htim13.Instance->CCR1;
		break;
	case 7:
		currentPwm = &htim14.Instance->CCR1;
		break;
	default:
		return;
	}

	
	int error=(*currentPwm > targetPwm[nServo] ? (*currentPwm - targetPwm[nServo]) : (targetPwm[nServo] - *currentPwm));
	int8_t flag = 1;
	if (*currentPwm > targetPwm[nServo])
		flag = -1; // 如果当前pwm值大于目标pwm值，将flag设置为-1
	if (*currentPwm == targetPwm[nServo])
		flag = 0; // 如果当前pwm值等于目标pwm值，将flag设置为0
	if(error > 45){
		flag = 10 * flag;}
	else if(error>25){
		flag = 5 * flag;}
	else if(error>10){
		flag = 1 * flag;}
//	if (*currentPwm > targetPwm[nServo] ? (*currentPwm - targetPwm[nServo] > 30) : (targetPwm[nServo] - *currentPwm > 30)) {
//    	// 处理差值大于10的情况,三目运算符 不能用abs，因为是无符号类似的变量
//		flag = 25 * flag; // 如果当前pwm值小于目标pwm值，它们之间的差值大于20，flag需要乘以13
//	}
	// 将当前pwm值加上flag，实现缓慢移动的效果
	*currentPwm = (*currentPwm) + flag;
}

/**
	*@brief 机械臂状态切换
	*@param nextState 下个状态
	*@param step 角度步进
*/



/**
 * @description: ArmSolution 机械臂位置解算，需要自己完成输入x，y坐标，机械臂自动伸到那个位置
 * @return {*}自动设置所有舵机的角度
 */
void ArmSolution(double x, double y)
{
}

/**
 * @description: 机械臂抓取程序，输入目标的位置，机械臂抓取
 * @param {*}自动根据目标位置进行整个抓取
 * @return {*}
 */
void Arm_Grab(uint8_t floor)
{
	switch(floor)
	{
		case 1:
		{
			State2State(Arm_Grab_floor1,2);
			while(!ServoTunnerOK());
			SetServoAngle(2,250); //catch
			while(!ServoTunnerOK());
			//catch&move to back
			//控制舵机 0 角度增加；回机械臂
			break;
		}
		case 2:
		{  
			State2State(Arm_Grab_floor2,2);
			while(!ServoTunnerOK());
			SetServoAngle(2,250); //catch
			while(!ServoTunnerOK());
			//舵机 0 角度减小； 舵机 1 角度增加； 收回机械臂
			break;	
		}
		case 3:
		{
			State2State(Arm_Grab_floor3,2);
			while(!ServoTunnerOK());
			SetServoAngle(2,250); //catch
			while(!ServoTunnerOK());
			//舵机 0 角度减小； 舵机 1 角度增加； 收回机械臂
			break;
		}
	}
}

void Arm_Detect(uint8_t floor)
{
	switch(floor)
	{
		case 0: //识别清单
		{
			StepperMotor_SetPosition(370);    //自上而下22.7cm
			State2State(Arm_Detect_floor0,2);
			SetServoAngle(2,80);
			break;
		}
		case 1:
		{
			StepperMotor_SetPosition(5);      //自上而下75.3cm       
			State2State(Arm_Detect_floor1,2);
			SetServoAngle(2,80);
			while(!ServoTunnerOK());
			break;
		}
		case 2:
		{
			StepperMotor_SetPosition(260);          //自上而下41.3cm
			State2State(Arm_Detect_floor2,2);
			SetServoAngle(2,80);
			while(!ServoTunnerOK());
			break;
		}
		case 3:
		{
			StepperMotor_SetPosition(397);        //自上而下16.3cm
			State2State(Arm_Detect_floor3,2);
			SetServoAngle(2,80);
			while(!ServoTunnerOK());
			break;
		}
	}
}

void Arm_Put(void)
{
			State2State(Arm_Putdown,2);
			while(!ServoTunnerOK());
			SetServoAngle(2,80);
			while(!ServoTunnerOK());
}

/**
	*@brief 舵机状态切换
	*@param nextState 下个状态
	*@param step 角度步进
*/
void State2State(int* nextState,double setStep){
	int flag[2] = {0,0};
	int maxAngle = 0;
	
	for(int i=0;i<2;i++){
		previous_arm_state[i] = current_arm_state[i];
		current_arm_state[i] = nextState[i];
		int angle = abs(current_arm_state[i]-previous_arm_state[i]);
		if(angle>0) 
			flag[i] = 1;//该舵机状态需要切换
		if(angle>maxAngle) 
			maxAngle = angle;
	}
	int iter = maxAngle/setStep +1;//迭代次数
	double step[2];//每个舵机的步进角度
	for(int i = 0;i<2;i++){
		step[i] = (current_arm_state[i]-previous_arm_state[i])*1.0/iter;
	}
	for(int i = 1;i<iter;i++){
		for(int j = 0;j<2;j++){
			if(flag[j]==1){

				int angle = previous_arm_state[j]+ i*step[j];
				SetServoAngle(j,angle);
				
			}
		}
		HAL_Delay(20);
	}
	//最终状态

	SetServoAngle(0,current_arm_state[0]);
	SetServoAngle(1,current_arm_state[1]);
	
	HAL_Delay(200);
}
