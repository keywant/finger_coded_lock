#include "head.h"



sbit  Car_Servo = P2^0;      // ���pwm����
u16 Servo0PwmDuty = 600;	//PWM������   1.5ms������  Ϊ�������λ��

void Timer0Init(void)		//20����@12.000MHz
{
	//AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0xE0;		//���ö�ʱ��ʼֵ
	TH0 = 0xB1;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ

	ET0 = 1;			//����ʱ��0�ж�
	EA = 1;       //�����ж�
}
void Car_Servo_Init()	//�����ʼ��
{
	Timer0Init();
}

void Timer0Value(u16 pwm)
{
	
	u16 value;
	value=0xffff-pwm;	
	TR0 = 0;
	TL0 = value;			//16λ���ݸ�8λ���ݸ�ֵĬ�Ͻ�16λ���ݵĵͰ�λֱ�Ӹ�����λ����
  TH0 = value >> 8;		//��16λ��������8λ��Ҳ���ǽ���8λ�Ƶ��Ͱ�λ���ٸ�ֵ��8λ����	
	TR0 = 1;
}
void Timer0_isr(void) interrupt 1 using 1
{
	
	static u8 i = 1;	//��̬������ÿ�ε��ú���ʱ������һ��������ֵ��
							//��ȫ�ֱ������ƣ���ͬ����ֻ�����ڴ˺����ڲ�
	
	switch(i)
	{
		case 1:
			Car_Servo = 1;	//PWM���ƽŸߵ�ƽ
			//����ʱ��0��ֵ������Pwm0Duty�����������жϣ��´��жϻ������һ��case���
			Timer0Value(Servo0PwmDuty);	
			break;
		case 2:
			Car_Servo = 0;	//PWM���ƽŵ͵�ƽ
			//�����������ʣ�µ�ʱ��(20000-Pwm0Duty)ȫ�ǵ͵�ƽ�ˣ�Pwm0Duty + (20000-Pwm0Duty) = 20000����������Ϊһ������20���� 
			Timer0Value(20000-Servo0PwmDuty);
			i = 0;	
			break;	 
	}
	i++;
}