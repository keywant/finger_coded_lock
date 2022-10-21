#include "head.h"


extern u8 KeyValue;			//����ֵ
extern char fla;				//��������һ���ź�
extern unsigned int Servo0PwmDuty;//PWM������



typedef struct {
	unsigned int password_insurance; //��������
	unsigned int password_open;		//�û�����
	unsigned int password;				//��ǰ��������ֵ
	unsigned int OPEN_time;				//�򿪴���
}codedlock_t;

codedlock_t chen = {815,0,0,0};

unsigned char count = 0; //���ְ������´���
int mode = 1;

int MODE_Choose()
{
	if(KeyValue == 4 && fla == 0)
	{
		LCD_ShowString(2,1," *key  finger");
		mode = 1;
		fla = -1;
	}
	if(KeyValue == 12 && fla == 0)
	{
		LCD_ShowString(2,1,"  key *finger");
		mode = 2;
		fla = -1;
	}
	if(KeyValue == 8 && fla == 0)
	{
		fla = -1;
		return mode;
	}
	return 0;
}

void PassWord_Input()		//�����ʼ��
{
//	EEPROM_write(0x05,1256/256);delay_1ms(10); 	//eepromд���û���ʼ���� ��һ��ʹ��д��һ�μ���
//	EEPROM_write(0x06,1256%256);delay_1ms(10);
//	EEPROM_write(0x04,0%256);delay_1ms(10); 		//eepromд�뿪������
	
		delay_1ms(100); 
		chen.password_open = EEPROM_read(0x05) * 256;delay_1ms(10);
		chen.password_open += EEPROM_read(0x06);delay_1ms(10); //�ϵ��ȡ�û�����
	
		chen.OPEN_time += EEPROM_read(0x04);delay_1ms(20);		//�ϵ��ȡ��������
	
}


void reset()  //�������ú���
{
	chen.password = 0;	//��ǰ��������ֵ����
	count = 0;		//���ְ������´�������
	LCD_ShowString(1,1,"===Coded Lock===");
	LCD_ShowString(2,1,"Input:          ");	//ˢ�½���
	fla = -1;      //�����ɶ�ȡ�ź�
}

void Key_Input() //���밴�����뺯��
{
	
	if (count < 4 && fla == 0 && KeyValue % 4 != 0 && KeyValue != 13 && KeyValue != 15) 
		{
			
				count += 1;
				chen.password *= 10; 
				switch(KeyValue)
				{
					case 5:chen.password += 4;break;
					case 6:chen.password += 5;break;
					case 7:chen.password += 6;break;	
					case 9:chen.password += 7;break;
					case 10:chen.password += 8;break;
					case 11:chen.password += 9;break;
					case 14:chen.password += 0;break;
					default:chen.password += KeyValue;break;
				}
				LCD_ShowNum(2,7,chen.password,count);
				fla = -1;
		}
}



void Key_Check()			//�����麯�� KeyValue=16
{
	
	if (count == 4 && KeyValue == 16 && fla == 0)		
	{
			if (chen.password == chen.password_open || (chen.password == chen.password_insurance))		//�û�������ȷ �����뱣������
			{
				open_door();
				chen.OPEN_time++;
				reset();
			}
			else       //�������
			{
				LCD_ShowString(2,14,"ERR");	//��ʾERR
				delay_1ms(1000);
				reset();
			}
		}
}
void Key_Change() //�����޸ĺ���	KeyValue=13
{
	unsigned int i = 0;
	if(KeyValue == 13 && fla == 0) 
	{
		fla = -1;
		delay_1ms(10);
		i = EEPROM_read(0x04);		//��ȡeeprom�еĿ�������
		if(i != chen.OPEN_time)		//���������뵱ǰ�����Ա�
		{
			LCD_ShowString(2,1,"PleaseChange");
			delay_1ms(1200);
			reset();
			while(1)
			{
				Key_Input();
				Key_Eliminate();
				if (count == 4 && KeyValue == 16 && fla == 0)
				{
					chen.password_open = chen.password;		
					delay_1ms(100);
					EEPROM_write(0x05,chen.password_open/256);delay_1ms(100); 
					EEPROM_write(0x06,chen.password_open%256);delay_1ms(100);
					if(chen.OPEN_time == 255) {chen.OPEN_time = 0;}
					EEPROM_write(0x04,chen.OPEN_time%256);delay_1ms(100);

					LCD_ShowString(2,11,"Change");
					delay_1ms(1200);
					reset();
					break;
				}
			}
		}
	}
}
void Key_Eliminate() //������� KeyValue=15
{
	if(KeyValue == 15 && fla == 0) 
	{
			reset();
	}
}

void Key_sudo()  //�����û����� KeyValue=8 
{
	static char sudo = 3;
	if(KeyValue == 8 && fla == 0 && sudo != 0)
	{
		sudo--;
		if(sudo == 0)
		{
			open_door();
		}
		reset();
	}
}
void open_door()  //����
{
		LCD_ShowString(2,13,"OPEN");	//��ʾOPEN		
		Servo0PwmDuty = 2350;
		delay_1ms(2000);
		Servo0PwmDuty = 600;
}

void Finger_main()
{							
	LCD_ShowString(1,1,"Finger Test    ");//Һ��������ʾ����
	LCD_ShowString(2,1,"                "); 
  Uart_Init();			//��ʼ������
	
 	delay_1ms(200);          //��ʱ200MS���ȴ�ָ��ģ�鸴λ
	Device_Check();		   	//У��ָ��ģ���Ƿ������ȷ��Һ��������Ӧ����ʾ
	delay_1ms(1000);			//�Խӳɹ�����ͣ��һ��ʱ��
	P1 = 0xF0; 
	LCD_ShowString(1,1,"  search finger ");
	LCD_ShowString(2,1,"  Add   Delete  "); 		
	while(1)
	{
		if(KeyValue == 16 && fla == 0)
			{
				fla = -1;
				break;
			}		
	    /**************���������ܽ���****************/
		

		if(local_date==0)
		{
			LCD_ShowString(1,1," *search finger ");
			LCD_ShowString(2,1,"  Add   Delete  "); 
		}
		else if(local_date==1)
		{
			LCD_ShowString(1,1,"  search finger ");
			LCD_ShowString(2,1," *Add   Delete  "); 
		}
		else if(local_date==2)
		{
			LCD_ShowString(1,1,"  search finger ");
			LCD_ShowString(2,1,"  Add  *Delete ");
		}			
		//ȷ�ϼ�
		if(KeyValue == 8 && fla == 0)
		{	 
		 	fla = -1;								
			switch(local_date)
			{
					case 0:  //����ָ��						
					FPM10A_Find_Fingerprint();																								
					break;	
					
					case 1:	 //���ָ��
					FPM10A_Add_Fingerprint();
					break; 					
					
					case 2:	//���ָ��
					FPM10A_Delete_All_Fingerprint();
		  		break;
				}
			}
		  //�л��� KeyValue == 12
			if(KeyValue == 12 && fla == 0)
			{
			 	fla = -1;			
	  	 	if (local_date <= 2)
				{

					if(++local_date == 3)local_date = 0;						
				}		
			}						
			delay_1ms(100); //��ʱ�ж�100MS���һ��		
	}
}