/**************************************************************************************************
** �� �� ����C8051f93x_Timer01.c
** ��    ��: xzy    �汾: v01     ��д����: 2013.04.26    �������: 2013.04.26
** ��������:1. ���������/��ʱ��T0��T1�����ڼ�������ʽ����ͨ�����濪��ʹ������T0 ��/�� T1
**			 2. ���������/��ʱ��T0��T1�������ſط�ʽ����ͨ�����濪��ʹ������INT0 ��/�� INT1
**			 3. ���ڶ�ʱ��01�����ڷ�ʽ1�����к���������ֱ�ӵ���
**              ���������������ʽ��Timer0SetValue��Timer1SetValue��Timer01ReadValue��Ҫ�����޸�
** ��Ҫ����: 1. void Timer01Init(uint8 Timer, uint8 TimerMode, uint8 ClkSource);	//��ʱ��0��1 ��ʼ�� 
**           2. void Timer0SetValue(uint16 Value);			//������/��ʱ��0 ���ó�ֵ
**           3. void Timer1SetValue(uint16 Value);			//������/��ʱ��1 ���ó�ֵ 
**           4. uint16 Timer01ReadValue(uint8 Timer);		//��ȡ������/��ʱ��0,1��ֵ
**           5. Timer0Open()								//��ʱ��0 ���� 
**			 6. Timer0Off()                      			//��ʱ��0 �ر�
**           7. Timer1Open()								//��ʱ��1 ���� 
**			 8. Timer1Off()                      			//��ʱ��1 �ر�           
**---------------------------------------------------------------------------------------------------
** Ӳ��ƽ̨: 
**           ML-EC5������;
**           
**																					 
** ���ƽ̨: KEIL4.10
**---------------------------------------------------------------------------------------------------
** �޸ļ�¼: 
** 			 
*****************************************************************************************************/
#include "C8051f93x_IOPort.h"
#include "C8051f93x_Timer01.h"
#include "C8051f93x_UART0.h"
#include "Interrupt.h"
#include "pin_define.h"

#if	TIMER0_INTERRUPT_EN > 0
uint8 	Time0IntFlag;
#endif

#if	TIMER1_INTERRUPT_EN > 0
uint8 	Time1IntFlag;
#endif

/******************************************************************************
** �� �� ��: Timer01Init
** ��������: ��ʱ��01 ��ʼ��
** �䡡  ��: uint8 Timer, uint8 TimerMode, uint8 ClkSource
**           Timer = 0: ѡ��ʱ��0; 
**                 = 1: ѡ��ʱ��1;
**			 TimerMode = TIMER01_MODE0_13BIT: ��ʽ0,13 λ������/��ʱ��
**                     = TIMER01_MODE1_16BIT: ��ʽ1,16 λ������/��ʱ�� 
**                     = TIMER01_MODE2_8BIT_RELOAD: ��ʽ2,�Զ���װ�ص�8 λ������/��ʱ��
**                     = TIMER01_MODE3_8BIT: ��ʽ3,˫8 λ������/��ʱ��(ֻ�ж�ʱ��0��Ч)
**			 ClkSource = TIMER01_CLKSOURCE_SYSCLK_12: ��ʱ��1/0 ������ʱ��Ϊϵͳʱ��/12
**                     = TIMER01_CLKSOURCE_SYSCLK_4: ��ʱ��1/0 ������ʱ��Ϊϵͳʱ��/4
**                     = TIMER01_CLKSOURCE_SYSCLK_48: ��ʱ��1/0 ������ʱ��Ϊϵͳʱ��/48
**                     = TIMER01_CLKSOURCE_EXTCLK_8: ��ʱ��1/0 ������ʱ��Ϊ�ⲿʱ��/8
**                     = TIMER01_CLKSOURCE_SYSCLK: ��ʱ��01 ֱ��ѡ��ϵͳʱ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04     	
******************************************************************************/ 
#if FUNC_TIME01_INIT_EN > 0
void Timer01Init(uint8 Timer, uint8 TimerMode, uint8 ClkSource)
{
	if(Timer == 0)							//����Ƕ�ʱ��0��ʼ��
	{
		TMOD  = (TMOD & 0x0F) | TimerMode;
		//TMOD |= Counter_Timer << 2;
#if TIMER0_COUNTER_TIMER > 0				//��ʱ��0 �����ڼ��������Ƕ�ʱ����ʽ
		TMOD |= 0x01 << 2;					//������ʽ

		/* ʹ��1 ������T0 */
		PortT0En();
	
#endif							

#if	TIMER0_GATE_CONTROL_EN > 0 				//�Ƿ�ʹ���ſط�ʽ
		TMOD |= 0x01 << 3;					//�ſ�
		if(TIMER0_GATE_CONTROL_LEVEL > 0)	//�ߵ�ƽ��Ч
		{
			IT01CF |= 0x08;
		}	
#endif

		if(ClkSource >= TIMER01_CLKSOURCE_SYSCLK)	//��ʱ��0/1 ֱ��ѡ��ϵͳʱ��
		{
			CKCON |= 0x04;
		}
		else								//��ʱ��0/1 Ԥ��Ƶʱ��
		{
			CKCON &= ~0x04;
			CKCON = (CKCON & (~0x03)) | ClkSource;
		}
		TH0	= 0x00;
		TL0 = 0x00;
	}
	else if(Timer == 1)						//����Ƕ�ʱ��1��ʼ��
	{
		TMOD  = (TMOD & 0xF0) | (TimerMode << 4);
		//TMOD |= Counter_Timer << 6;
#if TIMER1_COUNTER_TIMER > 0				//��ʱ��1 �����ڼ��������Ƕ�ʱ����ʽ
		TMOD |= 0x01 << 6;					//������ʽ
		/* ʹ��1 ������T1 */
		PortT1En();
#endif

#if	TIMER1_GATE_CONTROL_EN > 0 					//�Ƿ�ʹ���ſط�ʽ
		TMOD |= 0x01 << 7;						//�ſ�
		if(TIMER1_GATE_CONTROL_LEVEL > 0)	//�ߵ�ƽ��Ч
		{
			IT01CF |= 0x80;
		}		
#endif

		if(ClkSource >= TIMER01_CLKSOURCE_SYSCLK)	//��ʱ��0/1 ֱ��ѡ��ϵͳʱ��
		{
			CKCON |= 0x08;
		}
		else								//��ʱ��0/1 Ԥ��Ƶʱ��
		{
			CKCON &= ~0x08;
			CKCON = (CKCON & (~0x03)) | ClkSource;
		}

		TH1	= 0x00;
		TL1 = 0x00;
	}
}
#endif
		
/******************************************************************************
** �� �� ��: Timer0SetValue
** ��������: ������/��ʱ��0 ���ó�ֵ (ֻ�ǶԼ�����/��ʱ�������ڷ�ʽ1��16bit��ʽ)
** �䡡  ��: uint16 Value: TH0 * 256 + TL0
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04    	
******************************************************************************/ 
#if FUNC_TIME0_SETVALUE_EN > 0
void Timer0SetValue(uint16 Value)
{
	TH0 = (uint8)(Value / 256);
	TL0 = (uint8)(Value % 256);	
}
#endif	 

/******************************************************************************
** �� �� ��: Timer1SetValue
** ��������: ������/��ʱ��1 ���ó�ֵ(ֻ�ǶԼ�����/��ʱ�������ڷ�ʽ1��16bit��ʽ)
** �䡡  ��: uint16 Value: TH1 * 256 + TL1
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: 
** ��д����: 2009-5-13     	
******************************************************************************/ 
#if FUNC_TIME1_SETVALUE_EN > 0
void Timer1SetValue(uint16 Value)
{
	TH1 = (uint8)(Value / 256);
	TL1 = (uint8)(Value % 256);	
} 
#endif

/******************************************************************************
** �� �� ��: Timer01ReadValue
** ��������: ��ȡ������/��ʱ��0,1��ֵ(ֻ�ǶԼ�����/��ʱ�������ڷ�ʽ1��16bit��ʽ)
** �䡡  ��: uint8 Timer
**           Timer = 0: ѡ��ʱ��0; 
**                 = 1: ѡ��ʱ��1;
** �䡡  ��: uint16 Value
**			 Value: THn * 256 + TLn  (n = 0,1) 
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04     	
******************************************************************************/ 
#if	FUNC_TIMER01_READ_VALUE_EN > 0
uint16 Timer01ReadValue(uint8 Timer)
{
	uint16 Value;
	if(Timer == 0)
	{
		Value = (uint16)TL0 + 256 * TH0;   	
	}
	else// if(Timer == 1)
	{
		Value = (uint16)TL1 + 256 * TH1; 
	}
	return Value;
}
#endif
/*�жϿ���
#if TIMER0_INTERRUPT_EN > 0	
	Timer0IntSwitchOn();				//��������ж�
#endif
#if TIMER1_INTERRUPT_EN > 0	
	Timer1IntSwitchOn();				//��������ж�
#endif*/
/*****************************************************************************/
/*******************************�жϷ������**********************************/
/*****************************************************************************/

/******************************************************************************
** �� �� ��: Timer0Int
** ��������: ��ʱ��0�Ķ�ʱ�ж�(��ʱ50000����ʱ��ʱ��)
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ����ģ��: 
** ����  ��: 
** �ա�  ��: 2009-5-13
******************************************************************************/ 
#if	TIMER0_INTERRUPT_EN > 0
void Timer0Int(void) interrupt TIMER0_PRIOR_ORDER
{
	Timer0SetValue(0);				//0.5ms��ʱ		
	//UART0SendByte(buf[Time0IntFlag]);
	Time0IntFlag ++;
	if(Time0IntFlag >= 50)
	{
		USER_LED = ~USER_LED;
		Time0IntFlag = 0;
	}
}
#endif

/******************************************************************************
** �� �� ��: Timer1Int
** ��������: ��ʱ��1�Ķ�ʱ�ж�(��ʱ(��ʱ20000����ʱ��ʱ��))
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ����ģ��: 
** ����  ��: xzy
** �ա�  ��: 2013.04
******************************************************************************/ 
#if	TIMER1_INTERRUPT_EN > 0
void Timer1Int(void) interrupt TIMER1_PRIOR_ORDER
{
	Timer1SetValue(45536);				//0.2ms��ʱ	
	Time1IntFlag = FLAG_SET;
}
#endif

