
/**************************************************************************************************
** �� �� ����C8051f93x_Timer23.h
** ��    ��: xzy    �汾: v01     ��д����: 2013.04    �������: 2013
** ��������: 
**                    
**---------------------------------------------------------------------------------------------------
** Ӳ��ƽ̨: 
**           ML-EC5������;
**           
**																					 
** ���ƽ̨: KEIL4.12
**---------------------------------------------------------------------------------------------------
** �޸ļ�¼: 
** 			 
*****************************************************************************************************/
#ifndef 	C8051f93x_TIMER23_H
#define 	C8051f93x_TIMER23_H

#include "compiler_defs.h"             		// compiler declarations
#include "C8051F930_defs.h" 				// SFR declarations
#include "Compile.h"
#include "C8051f93x_SysClk.h"
#include "user_type.h"

#define		FLAG_SET 	1
#define		FLAG_CLR	0

#define		TIMER2		2
#define		TIMER3		3

#define		TIMER2_INTERRUPT_EN		1		//��ʱ��2�жϷ������
#define		TIMER3_INTERRUPT_EN		1		//��ʱ��3�жϷ������

#define TIMER_PRESCALER            12  // Based on Timer2 CKCON and TMR2CN settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  (SYSCLK/TIMER_PRESCALER)/1000
//#define TIMER_TICKS_PER_MS  (32768/8)/1000
                                        
#if	TIMER2_INTERRUPT_EN > 0
//extern	uint8	Time2IntFlag;
#endif				

#if	TIMER3_INTERRUPT_EN > 0
//extern	uint8	Time3IntFlag;
#endif


/* ��ʱ��2,3����ģʽ */
//��ʱ��3 �Ͷ�ʱ��2 ����T2 ��T2EX ����
#define		TIMER2_TIMER16_TIMER8		0			//��ʱ��2 ������16λ(0)/8λ(1)�Զ���װ�ض�ʱ�� 
#define		TIMER2_CAP_RELOAD			0			//��ʱ��2 �����ڲ�׽��ʽ(1)�����Զ���װ�ط�ʽ(0) 

#define		TIMER3_TIMER16_TIMER8		0			//��ʱ��3 ������16λ(0)/8λ(1)�Զ���װ�ض�ʱ�� 
#define		TIMER3_CAP_RELOAD			0			//��ʱ��3 �����ڲ�׽��ʽ(1)�����Զ���װ�ط�ʽ(0)

//��ʱ��2,3�ߵ��ֽ�ʱ��ѡ��
#define		TIMER2_ML_CLKSOURCE		0		//��ʱ�� 2 ���ֽ�ʱ��ѡ��0:T2XCLKλ�����ʱ��,1:ϵͳʱ��
#define		TIMER2_MH_CLKSOURCE		0		//��ʱ�� 2 ���ֽ�ʱ��ѡ��0:T2XCLKλ�����ʱ��,1:ϵͳʱ��
#define		TIMER3_ML_CLKSOURCE		0		//��ʱ�� 3 ���ֽ�ʱ��ѡ��0:T3XCLKλ�����ʱ��,1:ϵͳʱ��
#define		TIMER3_MH_CLKSOURCE		0		//��ʱ�� 3 ���ֽ�ʱ��ѡ��0:T3XCLKλ�����ʱ��,1:ϵͳʱ��
//�ⲿʱ��Դ��ϵͳʱ��ͬ����
//��ʱ��2 ������ʱ��Ϊ�ⲿʱ��Ϊ SYSCLK/12����׽����ʱ��Ϊ smaRTClock/8
//��ʱ��3 ������ʱ��Ϊ�ⲿʱ��Ϊ SYSCLK/12����׽����ʱ��Ϊ�Ƚ��� 1��
#define		TIMER23_CLKSOURCE_CLK_0		0

//��ʱ��2 ������ʱ��Ϊ�ⲿʱ��Ϊ�Ƚ��� 0����׽����ʱ��Ϊ smaRTClock/8��
//��ʱ��3 ������ʱ��Ϊ�ⲿʱ��Ϊ�ⲿ����/8����׽����ʱ��Ϊ�Ƚ��� 1��			
#define		TIMER23_CLKSOURCE_CLK_1		1

//��ʱ��2 ������ʱ��Ϊ�ⲿʱ��Ϊ SYSCLK/12����׽����ʱ��Ϊ�Ƚ��� 0��
//��ʱ��3 ������ʱ��Ϊ�ⲿʱ��Ϊ SYSCLK/12����׽����ʱ���ⲿ����/8�� 			
#define		TIMER23_CLKSOURCE_CLK_2		2

//��ʱ��2 ������ʱ��Ϊ�ⲿʱ��Ϊ�Ƚ��� 0����׽����ʱ��Ϊ smaRTClock/8��
//��ʱ��3 ������ʱ��Ϊ�ⲿʱ��Ϊ�Ƚ��� 1����׽����ʱ��Ϊ�ⲿ����/8��			
#define		TIMER23_CLKSOURCE_CLK_3		3			 

/* ���庯������ */
#define		FUNC_TIMER23_INIT_EN			1
#define		FUNC_TIMER23_SETVALUE_EN		1
#define		FUNC_TIMER23_SET_RL_VALUE_EN	1
#define		FUNC_TIMER23_READ_VALUE_EN	    0

#if FUNC_TIMER23_INIT_EN > 0
extern void Timer23Init(uint8 Timer, uint8 ClkSource);	//��ʱ��23 ��ʼ��
#endif

#if FUNC_TIMER23_SETVALUE_EN > 0
extern void Timer23SetValue(uint8 Timer, uint16 Value);		//������/��ʱ��23 ���ó�ֵ
#endif

#if FUNC_TIMER23_SET_RL_VALUE_EN > 0
extern void Timer23SetRLValue(uint8 Timer, uint16 Value);		//������/��ʱ��23 ������װ��ֵ
#endif

#if FUNC_TIMER23_READ_VALUE_EN > 0
extern uint16 Timer23ReadValue(uint8 Timer);			//��ȡ������/��ʱ��23��ֵ
#endif
//-----------------------------------------------------------------------------------
//˵������ʱ��2���ؿ���
//-----------------------------------------------------------------------------------
extern void EnableTimer2(void);
extern void DisableTimer2(void);
//-----------------------------------------------------------------------------------
//˵������ʱ��3���ؿ���
//-----------------------------------------------------------------------------------
extern void EnableTimer3(void);
extern void DisableTimer3(void);
/*
*****************************************************************************
** �� �� ��: void TIME_Delay(Uint8 Timer,Uint8 count)
** ��������: �ö�ʱ����ʱ
** �䡡  ��: Uint8 count��ʱʱ��ms(���42ms)
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/
extern void TIME_Delay(Uint8 Timer,Uint8 count);

//��ʱ��2 ����
#define	Timer2Open()	TR2   = 1

//��ʱ��2 �ر�
#define	Timer2Off()		TR2   = 0

//��ʱ��3 ����
#define	Timer3Open()	TMR3CN  |= (0x01 << 2)

//��ʱ��3 �ر�
#define	Timer3Off()		TMR3CN  &= (~(0x01 << 2))

//��ʱ��2 ��TF2H
#define	Timer2ClrTF2H()	TF2H   = 0

//��ʱ��2 ��TF2L
#define	Timer2ClrTF2L()	TF2L   = 0

//��ʱ��3 ��TF3H
#define	Timer3ClrTF3H()	TMR3CN  &= (~(0x01 << 7))

//��ʱ��3 ��TF3L
#define	Timer3ClrTF3L()	TMR3CN  &= (~(0x01 << 6))

//��ʱ��2 ˫8λ��ʽ����
#define Timer2Double8bitEN()	 T2SPLIT = 1

//��ʱ��2 ˫8λ��ʽ��ֹ
#define Timer2Double8bitDIS()	 T2SPLIT = 0

//��ʱ��3 ˫8λ��ʽ����
#define Timer3Double8bitEN()	 TMR3CN  |= (0x01 << 3)

//��ʱ��3 ˫8λ��ʽ��ֹ
#define Timer3Double8bitDIS()	 TMR3CN  &= (~(0x01 << 3))

//��ʱ��2 ��׽��ʽ����
#define Timer2CAPEN()			 TF2CEN = 1

//��ʱ��2 ��׽��ʽ��ֹ
#define Timer2CAPDIS()			 TF2CEN = 0

//��ʱ��3 ��׽��ʽ����
#define Timer3CAPEN()			 TMR3CN |= (0x01 << 4)

//��ʱ��3 ��׽��ʽ��ֹ
#define Timer3CAPDIS()			 TMR3CN &= (~(0x01 << 4))

#endif
