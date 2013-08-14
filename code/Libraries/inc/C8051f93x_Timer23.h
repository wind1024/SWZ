
/**************************************************************************************************
** 文 件 名：C8051f93x_Timer23.h
** 作    者: xzy    版本: v01     编写日期: 2013.04    完成日期: 2013
** 功能描述: 
**                    
**---------------------------------------------------------------------------------------------------
** 硬件平台: 
**           ML-EC5仿真器;
**           
**																					 
** 软件平台: KEIL4.12
**---------------------------------------------------------------------------------------------------
** 修改记录: 
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

#define		TIMER2_INTERRUPT_EN		1		//定时器2中断服务控制
#define		TIMER3_INTERRUPT_EN		1		//定时器3中断服务控制

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


/* 定时器2,3控制模式 */
//定时器3 和定时器2 共享T2 和T2EX 引脚
#define		TIMER2_TIMER16_TIMER8		0			//定时器2 工作于16位(0)/8位(1)自动重装载定时器 
#define		TIMER2_CAP_RELOAD			0			//定时器2 工作于捕捉方式(1)还是自动重装载方式(0) 

#define		TIMER3_TIMER16_TIMER8		0			//定时器3 工作于16位(0)/8位(1)自动重装载定时器 
#define		TIMER3_CAP_RELOAD			0			//定时器3 工作于捕捉方式(1)还是自动重装载方式(0)

//定时器2,3高低字节时钟选择
#define		TIMER2_ML_CLKSOURCE		0		//定时器 2 低字节时钟选择0:T2XCLK位定义的时钟,1:系统时钟
#define		TIMER2_MH_CLKSOURCE		0		//定时器 2 高字节时钟选择0:T2XCLK位定义的时钟,1:系统时钟
#define		TIMER3_ML_CLKSOURCE		0		//定时器 3 低字节时钟选择0:T3XCLK位定义的时钟,1:系统时钟
#define		TIMER3_MH_CLKSOURCE		0		//定时器 3 高字节时钟选择0:T3XCLK位定义的时钟,1:系统时钟
//外部时钟源与系统时钟同步。
//定时器2 的输入时钟为外部时钟为 SYSCLK/12。捕捉触发时钟为 smaRTClock/8
//定时器3 的输入时钟为外部时钟为 SYSCLK/12。捕捉触发时钟为比较器 1。
#define		TIMER23_CLKSOURCE_CLK_0		0

//定时器2 的输入时钟为外部时钟为比较器 0。捕捉触发时钟为 smaRTClock/8。
//定时器3 的输入时钟为外部时钟为外部振荡器/8。捕捉触发时钟为比较器 1。			
#define		TIMER23_CLKSOURCE_CLK_1		1

//定时器2 的输入时钟为外部时钟为 SYSCLK/12。捕捉触发时钟为比较器 0。
//定时器3 的输入时钟为外部时钟为 SYSCLK/12。捕捉触发时钟外部振荡器/8。 			
#define		TIMER23_CLKSOURCE_CLK_2		2

//定时器2 的输入时钟为外部时钟为比较器 0。捕捉触发时钟为 smaRTClock/8。
//定时器3 的输入时钟为外部时钟为比较器 1。捕捉触发时钟为外部振荡器/8。			
#define		TIMER23_CLKSOURCE_CLK_3		3			 

/* 具体函数声明 */
#define		FUNC_TIMER23_INIT_EN			1
#define		FUNC_TIMER23_SETVALUE_EN		1
#define		FUNC_TIMER23_SET_RL_VALUE_EN	1
#define		FUNC_TIMER23_READ_VALUE_EN	    0

#if FUNC_TIMER23_INIT_EN > 0
extern void Timer23Init(uint8 Timer, uint8 ClkSource);	//定时器23 初始化
#endif

#if FUNC_TIMER23_SETVALUE_EN > 0
extern void Timer23SetValue(uint8 Timer, uint16 Value);		//计数器/定时器23 设置初值
#endif

#if FUNC_TIMER23_SET_RL_VALUE_EN > 0
extern void Timer23SetRLValue(uint8 Timer, uint16 Value);		//计数器/定时器23 设置重装初值
#endif

#if FUNC_TIMER23_READ_VALUE_EN > 0
extern uint16 Timer23ReadValue(uint8 Timer);			//读取计数器/定时器23的值
#endif
//-----------------------------------------------------------------------------------
//说明：定时器2开关控制
//-----------------------------------------------------------------------------------
extern void EnableTimer2(void);
extern void DisableTimer2(void);
//-----------------------------------------------------------------------------------
//说明：定时器3开关控制
//-----------------------------------------------------------------------------------
extern void EnableTimer3(void);
extern void DisableTimer3(void);
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
extern void TIME_Delay(Uint8 Timer,Uint8 count);

//定时器2 开启
#define	Timer2Open()	TR2   = 1

//定时器2 关闭
#define	Timer2Off()		TR2   = 0

//定时器3 开启
#define	Timer3Open()	TMR3CN  |= (0x01 << 2)

//定时器3 关闭
#define	Timer3Off()		TMR3CN  &= (~(0x01 << 2))

//定时器2 清TF2H
#define	Timer2ClrTF2H()	TF2H   = 0

//定时器2 清TF2L
#define	Timer2ClrTF2L()	TF2L   = 0

//定时器3 清TF3H
#define	Timer3ClrTF3H()	TMR3CN  &= (~(0x01 << 7))

//定时器3 清TF3L
#define	Timer3ClrTF3L()	TMR3CN  &= (~(0x01 << 6))

//定时器2 双8位方式允许
#define Timer2Double8bitEN()	 T2SPLIT = 1

//定时器2 双8位方式禁止
#define Timer2Double8bitDIS()	 T2SPLIT = 0

//定时器3 双8位方式允许
#define Timer3Double8bitEN()	 TMR3CN  |= (0x01 << 3)

//定时器3 双8位方式禁止
#define Timer3Double8bitDIS()	 TMR3CN  &= (~(0x01 << 3))

//定时器2 捕捉方式允许
#define Timer2CAPEN()			 TF2CEN = 1

//定时器2 捕捉方式禁止
#define Timer2CAPDIS()			 TF2CEN = 0

//定时器3 捕捉方式允许
#define Timer3CAPEN()			 TMR3CN |= (0x01 << 4)

//定时器3 捕捉方式禁止
#define Timer3CAPDIS()			 TMR3CN &= (~(0x01 << 4))

#endif
