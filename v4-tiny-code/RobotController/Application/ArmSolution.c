
#include "ArmSolution.h"
#include "delay.h"
#include "math.h"
#include "tim.h"
#include "stdlib.h"
#include "Stepper_Motor.h"
#define PWM_DUTY_LIMIT 10000 // PWMռ�ձ�����Ϊ10000,����20ms    250-1250 ���� 0-180��

// ÿ����������ƶ���Ŀ��pwm
uint16_t targetPwm[8] = {250};


int current_arm_state[2] ={80,176};  //��е�۵�ǰ״̬
int previous_arm_state[2] ={80,176}; //��е���ϴ�״̬

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

	//ֱ�Ӹ�pwm����ֵ  ��ʼ�趨Ϊ��ʼ��е��״̬  ÿ���ϵ�ǰӦ�ðѻ�е�۹�λ
	//����Ϊʹ�þ���
	Servo_init(0,80);
	Servo_init(1,176);
	Servo_init(2,80);

}
void Servo_init(uint8_t nServo,int angle)
{
	volatile uint32_t *currentPwm;//ָ��ָ��ͬ��pwm���Ƶ�ַ������ͳһ�޸ġ�
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
	if (angle < 0) // �Ƕ�����
		return;
	int pwm = angle * 1.0f / 180 / 20 * PWM_DUTY_LIMIT + 250; // �������Ӧ��pwm��

	// pwm���ƣ���ֹ��������
	if (pwm > 1250)
		pwm = 1250;
	if (pwm < 250)
		pwm = 250;

	*currentPwm = pwm;	//���ö�ʱ��CCR��PWMֵ
	targetPwm[nServo]=pwm;	//����Ŀ��PWMֵ
}

/**
 * @description: ���ö���Ƕ�
 * @param {int} nServo���Ŷ��,��0-7
 * @param {float} angle�Ƕ�(0-180)
 * @return {*}
 */
void SetServoAngle(int nServo, float angle)
{
	if (angle < 0) // �Ƕ�����
		return;
	int pwm = angle * 1.0f / 180 / 20 * PWM_DUTY_LIMIT + 250; // �������Ӧ��pwm��

	// pwm���ƣ���ֹ��������
	if (pwm > 1250)
		pwm = 1250;
	if (pwm < 250)
		pwm = 250;

	targetPwm[nServo] = pwm;
}

/* �жϵ����Ƿ���� ��ÿ������Ƕ��Ƿ���Ŀ��һ�� 
	��Ҫ�����Լ�������ȥɾȥһЩû�õ��Ķ��pwm�ж�
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

/*��������ƶ����������ÿ�����Ҫ����������ı�*/
void slowPwm(uint8_t nServo)
{
	volatile uint32_t *currentPwm;//ָ��ָ��ͬ��pwm���Ƶ�ַ������ͳһ�޸ġ�
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
		flag = -1; // �����ǰpwmֵ����Ŀ��pwmֵ����flag����Ϊ-1
	if (*currentPwm == targetPwm[nServo])
		flag = 0; // �����ǰpwmֵ����Ŀ��pwmֵ����flag����Ϊ0
	if(error > 45){
		flag = 10 * flag;}
	else if(error>25){
		flag = 5 * flag;}
	else if(error>10){
		flag = 1 * flag;}
//	if (*currentPwm > targetPwm[nServo] ? (*currentPwm - targetPwm[nServo] > 30) : (targetPwm[nServo] - *currentPwm > 30)) {
//    	// �����ֵ����10�����,��Ŀ����� ������abs����Ϊ���޷������Ƶı���
//		flag = 25 * flag; // �����ǰpwmֵС��Ŀ��pwmֵ������֮��Ĳ�ֵ����20��flag��Ҫ����13
//	}
	// ����ǰpwmֵ����flag��ʵ�ֻ����ƶ���Ч��
	*currentPwm = (*currentPwm) + flag;
}

/**
	*@brief ��е��״̬�л�
	*@param nextState �¸�״̬
	*@param step �ǶȲ���
*/



/**
 * @description: ArmSolution ��е��λ�ý��㣬��Ҫ�Լ��������x��y���꣬��е���Զ��쵽�Ǹ�λ��
 * @return {*}�Զ��������ж���ĽǶ�
 */
void ArmSolution(double x, double y)
{
}

/**
 * @description: ��е��ץȡ��������Ŀ���λ�ã���е��ץȡ
 * @param {*}�Զ�����Ŀ��λ�ý�������ץȡ
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
			//���ƶ�� 0 �Ƕ����ӣ��ػ�е��
			break;
		}
		case 2:
		{  
			State2State(Arm_Grab_floor2,2);
			while(!ServoTunnerOK());
			SetServoAngle(2,250); //catch
			while(!ServoTunnerOK());
			//��� 0 �Ƕȼ�С�� ��� 1 �Ƕ����ӣ� �ջػ�е��
			break;	
		}
		case 3:
		{
			State2State(Arm_Grab_floor3,2);
			while(!ServoTunnerOK());
			SetServoAngle(2,250); //catch
			while(!ServoTunnerOK());
			//��� 0 �Ƕȼ�С�� ��� 1 �Ƕ����ӣ� �ջػ�е��
			break;
		}
	}
}

void Arm_Detect(uint8_t floor)
{
	switch(floor)
	{
		case 0: //ʶ���嵥
		{
			StepperMotor_SetPosition(370);    //���϶���22.7cm
			State2State(Arm_Detect_floor0,2);
			SetServoAngle(2,80);
			break;
		}
		case 1:
		{
			StepperMotor_SetPosition(5);      //���϶���75.3cm       
			State2State(Arm_Detect_floor1,2);
			SetServoAngle(2,80);
			while(!ServoTunnerOK());
			break;
		}
		case 2:
		{
			StepperMotor_SetPosition(260);          //���϶���41.3cm
			State2State(Arm_Detect_floor2,2);
			SetServoAngle(2,80);
			while(!ServoTunnerOK());
			break;
		}
		case 3:
		{
			StepperMotor_SetPosition(397);        //���϶���16.3cm
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
	*@brief ���״̬�л�
	*@param nextState �¸�״̬
	*@param step �ǶȲ���
*/
void State2State(int* nextState,double setStep){
	int flag[2] = {0,0};
	int maxAngle = 0;
	
	for(int i=0;i<2;i++){
		previous_arm_state[i] = current_arm_state[i];
		current_arm_state[i] = nextState[i];
		int angle = abs(current_arm_state[i]-previous_arm_state[i]);
		if(angle>0) 
			flag[i] = 1;//�ö��״̬��Ҫ�л�
		if(angle>maxAngle) 
			maxAngle = angle;
	}
	int iter = maxAngle/setStep +1;//��������
	double step[2];//ÿ������Ĳ����Ƕ�
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
	//����״̬

	SetServoAngle(0,current_arm_state[0]);
	SetServoAngle(1,current_arm_state[1]);
	
	HAL_Delay(200);
}
