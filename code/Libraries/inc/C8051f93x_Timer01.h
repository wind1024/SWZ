/**************************************************************************************************
** 文 件 名：C8051F930_Timer01.h
** 作    者: xzy     版本: v01     编写日期: 2013.04.26    完成日期: 2013.04.26
** 功能描述: 对于定时器01工作于方式1，所有函数都可以直接调用
**           如果工作于其他方式，Timer0SetValue，Timer1SetValue，Timer01ReadValue需要重新修改
** 主要函数: 1. void Timer01Init(uint8 Timer, uint8 TimerMode, uint8 ClkSource);	//定时器0、1 初始化 
**           2. void Timer0SetValue(uint16 Value);			//计数器/定时器0 设置初值
**           3. void Timer1SetValue(uint16 Value);			//计数器/定时器1 设置初值 
**           4. uint16 Timer01ReadValue(uint8 Timer);		//读取计数器/定时器0,1的值

**           5. Timer0Open()								//定时器0 开启 
**			 6. Timer0Off()                      			//定时器0 关闭
**           7. Timer1Open()								//定时器1 开启 
**			 8. Timer1Off()                      			//定时器1 关闭
*****************************************************************************************************/
#ifndef 	_TIMER_H
#define 	_TIMER_H

#include "compiler_defs.h"             		// compiler declarations
#include "C8051F930_defs.h" 				// SFR declarations
#include "Interrupt.h"
#include "Compile.h"	

#define		FLAG_SET 	1
#define		FLAG_CLR	0

#define		TIMER0_INTERRUPT_EN		0	   //定时器0中断服务控制
#define		TIMER1_INTERRUPT_EN		0	   //定时器1中断服务控制

#if	TIMER0_INTERRUPT_EN > 0
extern	uint8	Time0IntFlag;
#endif

#if	TIMER1_INTERRUPT_EN > 0
extern	uint8	Time1IntFlag;
#endif

/* 定时器0，1控制模式 */
#define		TIMER0_GATE_CONTROL_EN		0			//定时器0 是否使能门控方式 (1:使能)
#define		TIMER0_COUNTER_TIMER		0			//定时器0 工作于计数器（1）还是定时器（0）方式

#define		TIMER1_GATE_CONTROL_EN		0			//定时器1 是否使能门控方式 1:使能
#define		TIMER1_COUNTER_TIMER		0			//定时器1 工作于计数器（1）还是定时器（0）方式

#define		EXINT0_TRIGGER_MODE			0			//外部中断0中断触发类型(1:边沿触发,0:电平触发)
#define		EXINT1_TRIGGER_MODE			0			//外部中断1中断触发类型(1:边沿触发,0:电平触发)
	
#define		TIMER0_GATE_CONTROL_LEVEL	0			//定时器0 门控方式 (1:高电平有效,0:低电平有效)
#define		TIMER1_GATE_CONTROL_LEVEL	0			//定时器1 门控方式 (1:高电平有效,0:低电平有效 )

#define		TIMER01_MODE0_13BIT			0			//方式0：13 位计数器/定时器
#define		TIMER01_MODE1_16BIT			1			//方式1：16 位计数器/定时器
#define		TIMER01_MODE2_8BIT_RELOAD	2			//方式2：自动重装载的8 位计数器/定时器
#define		TIMER01_MODE3_8BIT			3			//方式3：双8 位计数器/定时器(只有定时器0有效)

#define		TIMER01_CLKSOURCE_SYSCLK_12	0			//定时器1/0 的输入时钟为系统时钟/12
#define		TIMER01_CLKSOURCE_SYSCLK_4	1			//定时器1/0 的输入时钟为系统时钟/4
#define		TIMER01_CLKSOURCE_SYSCLK_48	2			//定时器1/0 的输入时钟为系统时钟/48
#define		TIMER01_CLKSOURCE_EXTCLK_8	3			//定时器1/0 的输入时钟为外部时钟/8
#define		TIMER01_CLKSOURCE_SYSCLK	4			//定时器0/1 直接选择系统时钟
//#define		TIMER1_CLKSOURCE_SYSCLK		5			//定时器1 直接选择系统时钟

/* 具体函数声明 */
#define		FUNC_TIME01_INIT_EN			1			//定时器初始化控制（0：无效，1：有效）
#define		FUNC_TIME0_SETVALUE_EN		0			//定时器0 方式1(16位计数器/定时器)控制（0：非方式1,1：方式1） 
#define		FUNC_TIME1_SETVALUE_EN		1			//定时器1 方式1(16位计数器/定时器)控制（0：非方式1,1：方式1） 
#define		FUNC_TIMER01_READ_VALUE_EN	0			//读取计数器/定时器0,1的值(只是对计数器/定时器工作于方式1：16bit方式)(0：无效，1：有效)

#if FUNC_TIME01_INIT_EN > 0
extern void Timer01Init(uint8 Timer, uint8 TimerMode, uint8 ClkSource);	//定时器0、1 初始化
#endif

//extern void Timer01SetValue(uint8 Timer, uint16 Value); //计数器/定时器0,1 设置初值
//extern void Timer0SetValue(uint8 ValueTH, uint8 ValueTL); //计数器/定时器0 设置初值
//extern void Timer1SetValue(uint8 ValueTH, uint8 ValueTL); //计数器/定时器1 设置初值
#if FUNC_TIME0_SETVALUE_EN > 0
extern void Timer0SetValue(uint16 Value);			//计数器/定时器0 设置初值
#endif

#if FUNC_TIME1_SETVALUE_EN > 0
extern void Timer1SetValue(uint16 Value);			//计数器/定时器1 设置初值
#endif

#if FUNC_TIMER01_READ_VALUE_EN > 0
extern uint16 Timer01ReadValue(uint8 Timer);		//读取计数器/定时器0,1的值
#endif

//extern uint8 Timer0OverStat(void);					//判断定时器0溢出状态

//定时器0 开启
#define	Timer0Open()	TR0   = 1

//定时器0 关闭
#define	Timer0Off()		TR0   = 0

//定时器1 开启
#define	Timer1Open()	TR1   = 1

//定时器1 关闭
#define	Timer1Off()		TR1   = 0

#endif
