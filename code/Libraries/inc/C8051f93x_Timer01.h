/**************************************************************************************************
** �� �� ����C8051F930_Timer01.h
** ��    ��: xzy     �汾: v01     ��д����: 2013.04.26    �������: 2013.04.26
** ��������: ���ڶ�ʱ��01�����ڷ�ʽ1�����к���������ֱ�ӵ���
**           ���������������ʽ��Timer0SetValue��Timer1SetValue��Timer01ReadValue��Ҫ�����޸�
** ��Ҫ����: 1. void Timer01Init(uint8 Timer, uint8 TimerMode, uint8 ClkSource);	//��ʱ��0��1 ��ʼ�� 
**           2. void Timer0SetValue(uint16 Value);			//������/��ʱ��0 ���ó�ֵ
**           3. void Timer1SetValue(uint16 Value);			//������/��ʱ��1 ���ó�ֵ 
**           4. uint16 Timer01ReadValue(uint8 Timer);		//��ȡ������/��ʱ��0,1��ֵ

**           5. Timer0Open()								//��ʱ��0 ���� 
**			 6. Timer0Off()                      			//��ʱ��0 �ر�
**           7. Timer1Open()								//��ʱ��1 ���� 
**			 8. Timer1Off()                      			//��ʱ��1 �ر�
*****************************************************************************************************/
#ifndef 	_TIMER_H
#define 	_TIMER_H

#include "compiler_defs.h"             		// compiler declarations
#include "C8051F930_defs.h" 				// SFR declarations
#include "Interrupt.h"
#include "Compile.h"	

#define		FLAG_SET 	1
#define		FLAG_CLR	0

#define		TIMER0_INTERRUPT_EN		0	   //��ʱ��0�жϷ������
#define		TIMER1_INTERRUPT_EN		0	   //��ʱ��1�жϷ������

#if	TIMER0_INTERRUPT_EN > 0
extern	uint8	Time0IntFlag;
#endif

#if	TIMER1_INTERRUPT_EN > 0
extern	uint8	Time1IntFlag;
#endif

/* ��ʱ��0��1����ģʽ */
#define		TIMER0_GATE_CONTROL_EN		0			//��ʱ��0 �Ƿ�ʹ���ſط�ʽ (1:ʹ��)
#define		TIMER0_COUNTER_TIMER		0			//��ʱ��0 �����ڼ�������1�����Ƕ�ʱ����0����ʽ

#define		TIMER1_GATE_CONTROL_EN		0			//��ʱ��1 �Ƿ�ʹ���ſط�ʽ 1:ʹ��
#define		TIMER1_COUNTER_TIMER		0			//��ʱ��1 �����ڼ�������1�����Ƕ�ʱ����0����ʽ

#define		EXINT0_TRIGGER_MODE			0			//�ⲿ�ж�0�жϴ�������(1:���ش���,0:��ƽ����)
#define		EXINT1_TRIGGER_MODE			0			//�ⲿ�ж�1�жϴ�������(1:���ش���,0:��ƽ����)
	
#define		TIMER0_GATE_CONTROL_LEVEL	0			//��ʱ��0 �ſط�ʽ (1:�ߵ�ƽ��Ч,0:�͵�ƽ��Ч)
#define		TIMER1_GATE_CONTROL_LEVEL	0			//��ʱ��1 �ſط�ʽ (1:�ߵ�ƽ��Ч,0:�͵�ƽ��Ч )

#define		TIMER01_MODE0_13BIT			0			//��ʽ0��13 λ������/��ʱ��
#define		TIMER01_MODE1_16BIT			1			//��ʽ1��16 λ������/��ʱ��
#define		TIMER01_MODE2_8BIT_RELOAD	2			//��ʽ2���Զ���װ�ص�8 λ������/��ʱ��
#define		TIMER01_MODE3_8BIT			3			//��ʽ3��˫8 λ������/��ʱ��(ֻ�ж�ʱ��0��Ч)

#define		TIMER01_CLKSOURCE_SYSCLK_12	0			//��ʱ��1/0 ������ʱ��Ϊϵͳʱ��/12
#define		TIMER01_CLKSOURCE_SYSCLK_4	1			//��ʱ��1/0 ������ʱ��Ϊϵͳʱ��/4
#define		TIMER01_CLKSOURCE_SYSCLK_48	2			//��ʱ��1/0 ������ʱ��Ϊϵͳʱ��/48
#define		TIMER01_CLKSOURCE_EXTCLK_8	3			//��ʱ��1/0 ������ʱ��Ϊ�ⲿʱ��/8
#define		TIMER01_CLKSOURCE_SYSCLK	4			//��ʱ��0/1 ֱ��ѡ��ϵͳʱ��
//#define		TIMER1_CLKSOURCE_SYSCLK		5			//��ʱ��1 ֱ��ѡ��ϵͳʱ��

/* ���庯������ */
#define		FUNC_TIME01_INIT_EN			1			//��ʱ����ʼ�����ƣ�0����Ч��1����Ч��
#define		FUNC_TIME0_SETVALUE_EN		0			//��ʱ��0 ��ʽ1(16λ������/��ʱ��)���ƣ�0���Ƿ�ʽ1,1����ʽ1�� 
#define		FUNC_TIME1_SETVALUE_EN		1			//��ʱ��1 ��ʽ1(16λ������/��ʱ��)���ƣ�0���Ƿ�ʽ1,1����ʽ1�� 
#define		FUNC_TIMER01_READ_VALUE_EN	0			//��ȡ������/��ʱ��0,1��ֵ(ֻ�ǶԼ�����/��ʱ�������ڷ�ʽ1��16bit��ʽ)(0����Ч��1����Ч)

#if FUNC_TIME01_INIT_EN > 0
extern void Timer01Init(uint8 Timer, uint8 TimerMode, uint8 ClkSource);	//��ʱ��0��1 ��ʼ��
#endif

//extern void Timer01SetValue(uint8 Timer, uint16 Value); //������/��ʱ��0,1 ���ó�ֵ
//extern void Timer0SetValue(uint8 ValueTH, uint8 ValueTL); //������/��ʱ��0 ���ó�ֵ
//extern void Timer1SetValue(uint8 ValueTH, uint8 ValueTL); //������/��ʱ��1 ���ó�ֵ
#if FUNC_TIME0_SETVALUE_EN > 0
extern void Timer0SetValue(uint16 Value);			//������/��ʱ��0 ���ó�ֵ
#endif

#if FUNC_TIME1_SETVALUE_EN > 0
extern void Timer1SetValue(uint16 Value);			//������/��ʱ��1 ���ó�ֵ
#endif

#if FUNC_TIMER01_READ_VALUE_EN > 0
extern uint16 Timer01ReadValue(uint8 Timer);		//��ȡ������/��ʱ��0,1��ֵ
#endif

//extern uint8 Timer0OverStat(void);					//�ж϶�ʱ��0���״̬

//��ʱ��0 ����
#define	Timer0Open()	TR0   = 1

//��ʱ��0 �ر�
#define	Timer0Off()		TR0   = 0

//��ʱ��1 ����
#define	Timer1Open()	TR1   = 1

//��ʱ��1 �ر�
#define	Timer1Off()		TR1   = 0

#endif
