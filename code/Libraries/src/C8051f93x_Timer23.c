
/**************************************************************************************************
** �� �� ����C8051f93x_Timer23.c
** ��    ��: xzy    �汾: v01     ��д����: 2013.04.27    �������: 2013
** ��������: 
**                    
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
#include "C8051f93x_Timer23.h"
#include "Interrupt.h"
#include "memory.h"	
#include "pin_define.h"
#include "Acceleration.h"

#if	TIMER2_INTERRUPT_EN > 0
//uint8 	Time2IntFlag;
#endif

#if	TIMER3_INTERRUPT_EN > 0
uint8 	Time3IntFlag;
#endif

/******************************************************************************
** �� �� ��: Timer23Init
** ��������: ��ʱ��234 ��ʼ��
** �䡡  ��: uint8 Timer, uint8 ClkSource
**           Timer = 2: ѡ��ʱ��2; 
**                 = 3: ѡ��ʱ��3;
**			 ClkSource = TIMER23_CLKSOURCE_CLK_0 :���C8051f93x_Timer23.h
**                     = TIMER23_CLKSOURCE_CLK_1
**                     = TIMER23_CLKSOURCE_CLK_2
**                     = TIMER23_CLKSOURCE_CLK_3
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04.27     	
******************************************************************************/ 
#if FUNC_TIMER23_INIT_EN > 0
void Timer23Init(uint8 Timer, uint8 ClkSource)
{
	switch(Timer)
	{
		/* ��ʱ��2 */
	 	case TIMER2: 
			TMR2CN   = 0x00;
#if TIMER2_CAP_RELOAD > 0
			Timer2Double8bitDIS();
#else
	#if TIMER2_TIMER16_TIMER8 > 0  //8λ��ʽ
			Timer2Double8bitEN();	
	#endif
#endif
#if	TIMER2_ML_CLKSOURCE >0
			CKCON |= 0x10;
#else 
			CKCON &= (~0x10);
#endif
#if	TIMER2_MH_CLKSOURCE >0
			CKCON |= 0x20;
#else 
			CKCON &= (~0x20);
#endif
			TMR2CN &= ~0x03; 
			TMR2CN = (TMR2CN & (~0x03)) | (ClkSource);
			TMR2L = 0x00;
			TMR2H = 0x00;
			
			TMR2RLL  = 0x00;
			TMR2RLH  = 0x00;						
			break;
		/* ��ʱ��3 */
		case TIMER3: 
			TMR3CN   = 0x00;
#if TIMER3_CAP_RELOAD > 0
			Timer3Double8bitDIS();
#else
	#if TIMER3_TIMER16_TIMER8 > 0  //8λ��ʽ
			Timer3Double8bitEN();	
	#endif
#endif
#if	TIMER3_ML_CLKSOURCE >0
			CKCON |= 0x40;
#else 
			CKCON &= (~0x40);
#endif
#if	TIMER3_MH_CLKSOURCE >0
			CKCON |= 0x80;
#else 
			CKCON &= (~0x80);
#endif
			TMR3CN &= ~0x03; 
			TMR3CN = (TMR3CN & (~0x03)) | (ClkSource);
			TMR3L = 0x00;
			TMR3H = 0x00;
			
			TMR3RLL  = 0x00;
			TMR3RLH  = 0x00;
			break;
		default: 	
			break;
	}	
}
#endif
		
/******************************************************************************
** �� �� ��: Timer23SetValue
** ��������: ������/��ʱ��23 ���ó�ֵ
** �䡡  ��: uint8 Timer, uint8 Value
**           Timer = 2: ѡ��ʱ��2; 
**                 = 3: ѡ��ʱ��3;
**           Value: TH0 * 256 + TL0
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04.27     	
******************************************************************************/ 
#if FUNC_TIMER23_SETVALUE_EN > 0
void Timer23SetValue(uint8 Timer, uint16 Value)
{
	switch(Timer)
	{
		/* ��ʱ��2 */
	 	case TIMER2: 
			TMR2H = (uint8)(Value / 256); 
			TMR2L = (uint8)(Value % 256);	
			break; 
		/* ��ʱ��3 */
	 	case TIMER3: 
			TMR3H = (uint8)(Value / 256); 
			TMR3L = (uint8)(Value % 256);
			break; 
		default: 	
			break;
	}	
}
#endif

/******************************************************************************
** �� �� ��: Timer23SetRLValue
** ��������: ������/��ʱ��23 ������װ��ֵ
** �䡡  ��: uint8 Timer, uint16 Value
**           Timer = 2: ѡ��ʱ��2; 
**                 = 3: ѡ��ʱ��3;
**           Value: TH0 * 256 + TL0
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04.27    	
******************************************************************************/ 
#if FUNC_TIMER23_SET_RL_VALUE_EN > 0
void Timer23SetRLValue(uint8 Timer, uint16 Value)
{
	switch(Timer)
	{
		/* ��ʱ��2 */
	 	case TIMER2: 
			TMR2RLH = (uint8)(Value / 256); 
			TMR2RLL = (uint8)(Value % 256);	
			break; 
		/* ��ʱ��3 */
	 	case TIMER3: 
			TMR3RLH = (uint8)(Value / 256); 
			TMR3RLL = (uint8)(Value % 256);
			break; 
		default: 	
			break;
	}	
}
#endif	


/******************************************************************************
** �� �� ��: Timer23ReadValue
** ��������: ��ȡ������/��ʱ��2,3��ֵ(ֻ�ǶԼ�����/��ʱ�������ڷ�ʽ1��16bit��ʽ)
** �䡡  ��: uint8 Timer
**           Timer = TIMER2: ѡ��ʱ��2; 
**                 = TIMER3: ѡ��ʱ��3;
** �䡡  ��: uint16 Value
**			 Value: THn * 256 + TLn  (n = 0,1) 
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04.27    	
******************************************************************************/ 
#if	FUNC_TIMER23_READ_VALUE_EN > 0
uint16 Timer23ReadValue(uint8 Timer)
{
	uint16 Value;
	switch(Timer)
	{
		/* ��ʱ��2 */
	 	case TIMER2:  
			Value = TMR2L + 256 * TMR2H;
			break; 
		/* ��ʱ��3 */
	 	case TIMER3: 
			Value = TMR3L + 256 * TMR3H;
			break; 
		default: 	
			break;
	}	
	return Value;
}
#endif
//-----------------------------------------------------------------------------------
//˵������ʱ��2���ؿ���
//-----------------------------------------------------------------------------------
void EnableTimer2(void)
{
	Timer2IntSwitchOn();
	Timer2Open();
}
void DisableTimer2(void)
{
	Timer2IntSwitchOff();
	Timer2Off();
}
//-----------------------------------------------------------------------------------
//˵������ʱ��3���ؿ���
//-----------------------------------------------------------------------------------
void EnableTimer3(void)
{
	Timer3IntSwitchOn();
	Timer3Open();
}
void DisableTimer3(void)
{
	Timer3IntSwitchOff();
	Timer3Off();
}
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
void TIME_Delay(Uint8 Timer,Uint8 count)
{
	switch(Timer)
	{
		case 2:
#if FUNC_TIMER23_SETVALUE_EN > 0	
	Timer23SetValue(2,65536 - count*TIMER_TICKS_PER_MS);
#endif
#if FUNC_TIMER23_SET_RL_VALUE_EN > 0
	Timer23SetRLValue(2,65536 - count*TIMER_TICKS_PER_MS);
#endif
#if	TIMER2_INTERRUPT_EN > 0
	//Timer2Open();
#endif
			break;
		case 3:
#if FUNC_TIMER23_SETVALUE_EN > 0	
	Timer23SetValue(3,65536 - count*TIMER_TICKS_PER_MS);
#endif
#if FUNC_TIMER23_SET_RL_VALUE_EN > 0
	Timer23SetRLValue(3,65536 - count*TIMER_TICKS_PER_MS);
#endif			
			break;
		default: 	
			break;
	}
}

/*****************************************************************************/
/*******************************�жϷ������**********************************/
/*****************************************************************************/

/******************************************************************************
** �� �� ��: Timer2Int
** ��������: ��ʱ��2�Ķ�ʱ�ж�
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ����ģ��: 
** ����  ��: xzy
** �ա�  ��: 2013.04.27
******************************************************************************/ 
#if	TIMER2_INTERRUPT_EN > 0
void Timer2Int(void) interrupt TIMER2_PRIOR_ORDER                       //20ms
{	
	static unsigned char tenMs  = 0;
	GlobleIntSwitchOff();
	Timer2ClrTF2H();
	Timer2ClrTF2L();
	
	SysDat.MainLoopError++;
	if(SysDat.SecCount && (ReadMask(SysDat.Flag,SEC_COUNT_MASK) == SEC_COUNT_MASK))	
	{
		tenMs ++;
		if(tenMs%50 == 0)
		{
			tenMs = 0;
			SysDat.SecCount --;
		}
	}
	if(SysDat.MainLoopError > 15000)		//>5 Min restart at once
	{
		SysDat.MainLoopError=0;
			//RSTSRC |= 0x10;      //RSTSRC����ʹ��λ������ֵ
		RSTSRC = 0x10;
	}
	//}
	//Time2IntFlag = FLAG_SET;
	GlobleIntSwitchOn();
}
#endif 

/******************************************************************************
** �� �� ��: Timer3Int
** ��������: ��ʱ��3�Ķ�ʱ�ж�
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ����ģ��: 
** ����  ��: xzy
** �ա�  ��: 2013.04.27
******************************************************************************/ 
#if	TIMER3_INTERRUPT_EN > 0
void Timer3Int(void) interrupt TIMER3_PRIOR_ORDER                    //10ms
{	
	static BYTE xdata Timer3_Counter = 0;
	GlobleIntSwitchOff();
	Timer3ClrTF3H();
	Timer3ClrTF3L();
	if(SysDat.flagGather)     //���ݲɼ��ö�ʱ
	{
		if(SysDat.timeLeft)	
		{
			SysDat.timeLeft -- ;			
		}
		if(SysDat.timeLeft == 0)
		{
			Timer3Off();
		}
	}
	else if(ReadMask(SysDat.Flag,TIME_ON_MASK) == TIME_ON_MASK)       //GPRS�����ö�ʱ
	{
		if(Timer3_Counter<100)									//Wait 1s 	
		Timer3_Counter++;
		else
		{
			Timer3_Counter = 0;

			/*if(SysDat.Link_Timer)
			{
				SysDat.Link_Timer --;
			}*/
			if(SysDat.ResendTimer)
			{
				SysDat.ResendTimer --;
				if(SysDat.ResendTimer < 1)
				{
					EXCH_SEND4_Flag=1;
				}
			}
			if(SysDat.TimerTest)
			{
				SysDat.TimerTest --;
			}
		}		
	}
	GlobleIntSwitchOn();	
}
#endif