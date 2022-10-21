#include "head.h"

extern u8 KeyValue;
extern char fla;

void main()
{
	int MODE = 0;
	LCD_Init();				//1602��ʼ��
	Key_Init();				//������ʼ��
	Car_Servo_Init(); //�����ʼ��
	LCD_ShowString(1,1," Please Choose"); 
	LCD_ShowString(2,1," *key  finger ");
	
	while(MODE == 0) //ģʽѡ��
	{
		MODE = MODE_Choose();
	}
	
	if(MODE == 1)
	{
		PassWord_Input(); //�����ʼ��
		/***������ʾ***/
		LCD_ShowString(1,1,"===Coded Lock===");
		LCD_ShowString(2,1,"Input:          ");
	
		while(1)
		{	
			if(KeyValue == 12 && fla == 0) 
			{
					fla = -1;
					break;
			}
			
			Key_Input();
			Key_Check();
			Key_Eliminate();
			Key_Change();
			Key_sudo();
		}
	}
	if(MODE == 2)
	{
			Finger_main();
	}
}
