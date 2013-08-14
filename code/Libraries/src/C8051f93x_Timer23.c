
/**************************************************************************************************
** 文 件 名：C8051f93x_Timer23.c
** 作    者: xzy    版本: v01     编写日期: 2013.04.27    完成日期: 2013
** 功能描述: 
**                    
**---------------------------------------------------------------------------------------------------
** 硬件平台: 
**           ML-EC5仿真器;
**           
**																					 
** 软件平台: KEIL4.10
**---------------------------------------------------------------------------------------------------
** 修改记录: 
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
** 函 数 名: Timer23Init
** 功能描述: 定时器234 初始化
** 输　  入: uint8 Timer, uint8 ClkSource
**           Timer = 2: 选择定时器2; 
**                 = 3: 选择定时器3;
**			 ClkSource = TIMER23_CLKSOURCE_CLK_0 :详见C8051f93x_Timer23.h
**                     = TIMER23_CLKSOURCE_CLK_1
**                     = TIMER23_CLKSOURCE_CLK_2
**                     = TIMER23_CLKSOURCE_CLK_3
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04.27     	
******************************************************************************/ 
#if FUNC_TIMER23_INIT_EN > 0
void Timer23Init(uint8 Timer, uint8 ClkSource)
{
	switch(Timer)
	{
		/* 定时器2 */
	 	case TIMER2: 
			TMR2CN   = 0x00;
#if TIMER2_CAP_RELOAD > 0
			Timer2Double8bitDIS();
#else
	#if TIMER2_TIMER16_TIMER8 > 0  //8位方式
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
		/* 定时器3 */
		case TIMER3: 
			TMR3CN   = 0x00;
#if TIMER3_CAP_RELOAD > 0
			Timer3Double8bitDIS();
#else
	#if TIMER3_TIMER16_TIMER8 > 0  //8位方式
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
** 函 数 名: Timer23SetValue
** 功能描述: 计数器/定时器23 设置初值
** 输　  入: uint8 Timer, uint8 Value
**           Timer = 2: 选择定时器2; 
**                 = 3: 选择定时器3;
**           Value: TH0 * 256 + TL0
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04.27     	
******************************************************************************/ 
#if FUNC_TIMER23_SETVALUE_EN > 0
void Timer23SetValue(uint8 Timer, uint16 Value)
{
	switch(Timer)
	{
		/* 定时器2 */
	 	case TIMER2: 
			TMR2H = (uint8)(Value / 256); 
			TMR2L = (uint8)(Value % 256);	
			break; 
		/* 定时器3 */
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
** 函 数 名: Timer23SetRLValue
** 功能描述: 计数器/定时器23 设置重装初值
** 输　  入: uint8 Timer, uint16 Value
**           Timer = 2: 选择定时器2; 
**                 = 3: 选择定时器3;
**           Value: TH0 * 256 + TL0
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04.27    	
******************************************************************************/ 
#if FUNC_TIMER23_SET_RL_VALUE_EN > 0
void Timer23SetRLValue(uint8 Timer, uint16 Value)
{
	switch(Timer)
	{
		/* 定时器2 */
	 	case TIMER2: 
			TMR2RLH = (uint8)(Value / 256); 
			TMR2RLL = (uint8)(Value % 256);	
			break; 
		/* 定时器3 */
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
** 函 数 名: Timer23ReadValue
** 功能描述: 读取计数器/定时器2,3的值(只是对计数器/定时器工作于方式1：16bit方式)
** 输　  入: uint8 Timer
**           Timer = TIMER2: 选择定时器2; 
**                 = TIMER3: 选择定时器3;
** 输　  出: uint16 Value
**			 Value: THn * 256 + TLn  (n = 0,1) 
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04.27    	
******************************************************************************/ 
#if	FUNC_TIMER23_READ_VALUE_EN > 0
uint16 Timer23ReadValue(uint8 Timer)
{
	uint16 Value;
	switch(Timer)
	{
		/* 定时器2 */
	 	case TIMER2:  
			Value = TMR2L + 256 * TMR2H;
			break; 
		/* 定时器3 */
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
//说明：定时器2开关控制
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
//说明：定时器3开关控制
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
** 函 数 名: void TIME_Delay(Uint8 Timer,Uint8 count)
** 功能描述: 用定时器延时
** 输　  入: Uint8 count延时时间ms(最大42ms)
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
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
/*******************************中断服务程序**********************************/
/*****************************************************************************/

/******************************************************************************
** 函 数 名: Timer2Int
** 功能描述: 定时器2的定时中断
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 调用模块: 
** 作　  者: xzy
** 日　  期: 2013.04.27
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
			//RSTSRC |= 0x10;      //RSTSRC不能使用位操作赋值
		RSTSRC = 0x10;
	}
	//}
	//Time2IntFlag = FLAG_SET;
	GlobleIntSwitchOn();
}
#endif 

/******************************************************************************
** 函 数 名: Timer3Int
** 功能描述: 定时器3的定时中断
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 调用模块: 
** 作　  者: xzy
** 日　  期: 2013.04.27
******************************************************************************/ 
#if	TIMER3_INTERRUPT_EN > 0
void Timer3Int(void) interrupt TIMER3_PRIOR_ORDER                    //10ms
{	
	static BYTE xdata Timer3_Counter = 0;
	GlobleIntSwitchOff();
	Timer3ClrTF3H();
	Timer3ClrTF3L();
	if(SysDat.flagGather)     //数据采集用定时
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
	else if(ReadMask(SysDat.Flag,TIME_ON_MASK) == TIME_ON_MASK)       //GPRS连接用定时
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