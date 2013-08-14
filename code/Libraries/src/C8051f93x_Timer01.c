/**************************************************************************************************
** 文 件 名：C8051f93x_Timer01.c
** 作    者: xzy    版本: v01     编写日期: 2013.04.26    完成日期: 2013.04.26
** 功能描述:1. 如果计数器/定时器T0、T1工作于计数器方式，则通过交叉开关使用引脚T0 和/或 T1
**			 2. 如果计数器/定时器T0、T1工作于门控方式，则通过交叉开关使用引脚INT0 和/或 INT1
**			 3. 对于定时器01工作于方式1，所有函数都可以直接调用
**              如果工作于其他方式，Timer0SetValue，Timer1SetValue，Timer01ReadValue需要重新修改
** 主要函数: 1. void Timer01Init(uint8 Timer, uint8 TimerMode, uint8 ClkSource);	//定时器0、1 初始化 
**           2. void Timer0SetValue(uint16 Value);			//计数器/定时器0 设置初值
**           3. void Timer1SetValue(uint16 Value);			//计数器/定时器1 设置初值 
**           4. uint16 Timer01ReadValue(uint8 Timer);		//读取计数器/定时器0,1的值
**           5. Timer0Open()								//定时器0 开启 
**			 6. Timer0Off()                      			//定时器0 关闭
**           7. Timer1Open()								//定时器1 开启 
**			 8. Timer1Off()                      			//定时器1 关闭           
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
** 函 数 名: Timer01Init
** 功能描述: 定时器01 初始化
** 输　  入: uint8 Timer, uint8 TimerMode, uint8 ClkSource
**           Timer = 0: 选择定时器0; 
**                 = 1: 选择定时器1;
**			 TimerMode = TIMER01_MODE0_13BIT: 方式0,13 位计数器/定时器
**                     = TIMER01_MODE1_16BIT: 方式1,16 位计数器/定时器 
**                     = TIMER01_MODE2_8BIT_RELOAD: 方式2,自动重装载的8 位计数器/定时器
**                     = TIMER01_MODE3_8BIT: 方式3,双8 位计数器/定时器(只有定时器0有效)
**			 ClkSource = TIMER01_CLKSOURCE_SYSCLK_12: 定时器1/0 的输入时钟为系统时钟/12
**                     = TIMER01_CLKSOURCE_SYSCLK_4: 定时器1/0 的输入时钟为系统时钟/4
**                     = TIMER01_CLKSOURCE_SYSCLK_48: 定时器1/0 的输入时钟为系统时钟/48
**                     = TIMER01_CLKSOURCE_EXTCLK_8: 定时器1/0 的输入时钟为外部时钟/8
**                     = TIMER01_CLKSOURCE_SYSCLK: 定时器01 直接选择系统时钟
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04     	
******************************************************************************/ 
#if FUNC_TIME01_INIT_EN > 0
void Timer01Init(uint8 Timer, uint8 TimerMode, uint8 ClkSource)
{
	if(Timer == 0)							//如果是定时器0初始化
	{
		TMOD  = (TMOD & 0x0F) | TimerMode;
		//TMOD |= Counter_Timer << 2;
#if TIMER0_COUNTER_TIMER > 0				//定时器0 工作于计数器还是定时器方式
		TMOD |= 0x01 << 2;					//计数方式

		/* 使用1 个引脚T0 */
		PortT0En();
	
#endif							

#if	TIMER0_GATE_CONTROL_EN > 0 				//是否使能门控方式
		TMOD |= 0x01 << 3;					//门控
		if(TIMER0_GATE_CONTROL_LEVEL > 0)	//高电平有效
		{
			IT01CF |= 0x08;
		}	
#endif

		if(ClkSource >= TIMER01_CLKSOURCE_SYSCLK)	//定时器0/1 直接选择系统时钟
		{
			CKCON |= 0x04;
		}
		else								//定时器0/1 预分频时钟
		{
			CKCON &= ~0x04;
			CKCON = (CKCON & (~0x03)) | ClkSource;
		}
		TH0	= 0x00;
		TL0 = 0x00;
	}
	else if(Timer == 1)						//如果是定时器1初始化
	{
		TMOD  = (TMOD & 0xF0) | (TimerMode << 4);
		//TMOD |= Counter_Timer << 6;
#if TIMER1_COUNTER_TIMER > 0				//定时器1 工作于计数器还是定时器方式
		TMOD |= 0x01 << 6;					//计数方式
		/* 使用1 个引脚T1 */
		PortT1En();
#endif

#if	TIMER1_GATE_CONTROL_EN > 0 					//是否使能门控方式
		TMOD |= 0x01 << 7;						//门控
		if(TIMER1_GATE_CONTROL_LEVEL > 0)	//高电平有效
		{
			IT01CF |= 0x80;
		}		
#endif

		if(ClkSource >= TIMER01_CLKSOURCE_SYSCLK)	//定时器0/1 直接选择系统时钟
		{
			CKCON |= 0x08;
		}
		else								//定时器0/1 预分频时钟
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
** 函 数 名: Timer0SetValue
** 功能描述: 计数器/定时器0 设置初值 (只是对计数器/定时器工作于方式1：16bit方式)
** 输　  入: uint16 Value: TH0 * 256 + TL0
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04    	
******************************************************************************/ 
#if FUNC_TIME0_SETVALUE_EN > 0
void Timer0SetValue(uint16 Value)
{
	TH0 = (uint8)(Value / 256);
	TL0 = (uint8)(Value % 256);	
}
#endif	 

/******************************************************************************
** 函 数 名: Timer1SetValue
** 功能描述: 计数器/定时器1 设置初值(只是对计数器/定时器工作于方式1：16bit方式)
** 输　  入: uint16 Value: TH1 * 256 + TL1
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: 
** 编写日期: 2009-5-13     	
******************************************************************************/ 
#if FUNC_TIME1_SETVALUE_EN > 0
void Timer1SetValue(uint16 Value)
{
	TH1 = (uint8)(Value / 256);
	TL1 = (uint8)(Value % 256);	
} 
#endif

/******************************************************************************
** 函 数 名: Timer01ReadValue
** 功能描述: 读取计数器/定时器0,1的值(只是对计数器/定时器工作于方式1：16bit方式)
** 输　  入: uint8 Timer
**           Timer = 0: 选择定时器0; 
**                 = 1: 选择定时器1;
** 输　  出: uint16 Value
**			 Value: THn * 256 + TLn  (n = 0,1) 
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04     	
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
/*中断控制
#if TIMER0_INTERRUPT_EN > 0	
	Timer0IntSwitchOn();				//开启溢出中断
#endif
#if TIMER1_INTERRUPT_EN > 0	
	Timer1IntSwitchOn();				//开启溢出中断
#endif*/
/*****************************************************************************/
/*******************************中断服务程序**********************************/
/*****************************************************************************/

/******************************************************************************
** 函 数 名: Timer0Int
** 功能描述: 定时器0的定时中断(定时50000个定时器时钟)
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 调用模块: 
** 作　  者: 
** 日　  期: 2009-5-13
******************************************************************************/ 
#if	TIMER0_INTERRUPT_EN > 0
void Timer0Int(void) interrupt TIMER0_PRIOR_ORDER
{
	Timer0SetValue(0);				//0.5ms定时		
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
** 函 数 名: Timer1Int
** 功能描述: 定时器1的定时中断(定时(定时20000个定时器时钟))
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 调用模块: 
** 作　  者: xzy
** 日　  期: 2013.04
******************************************************************************/ 
#if	TIMER1_INTERRUPT_EN > 0
void Timer1Int(void) interrupt TIMER1_PRIOR_ORDER
{
	Timer1SetValue(45536);				//0.2ms定时	
	Time1IntFlag = FLAG_SET;
}
#endif

