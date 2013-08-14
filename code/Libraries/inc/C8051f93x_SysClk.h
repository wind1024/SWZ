/***********************************************************************************
**文件说明：C8051f93x时钟系统配置头文件                                                              
**文件名称：C8051f93x_SysClk.h                                                      
**创建日期：2013.04                                                            
**创 建 人：XZY                                                                   
**---------------------------------------------------------------------------------*
**修改日期：                                                                       
**修改说明：                                                                       
***********************************************************************************/
#ifndef 	C8051F93X_SYSCLK_H
#define 	C8051F93X_SYSCLK_H

#include <compiler_defs.h>             		// compiler declarations
#include <C8051F930_defs.h> 				// SFR declarations
#include <Compile.h>

#define 	SYSCLK           18432000      	// SYSCLK 频率 单位Hz
/* 振荡器模式选择 */
#define		CLK_INT_OSC		0		//使用精准内部振荡器时钟
#define		CLK_EXT_OSC		1		//使用外部晶体振荡器时钟
#define		CLK_smaRTClock_OSC	3	    //使用smaRTClock振荡器时钟
#define		CLK_LOW_POWER_OSC	4		//使用低功耗内部振荡器时钟
								

/* 系统时钟分频选择 */
#define		OSC_SYSCLK_Div1		0x00
#define 	OSC_SYSCLK_Div2		0x10 
#define 	OSC_SYSCLK_Div4		0x20 
#define 	OSC_SYSCLK_Div8		0x30 
#define 	OSC_SYSCLK_Div16	0x40 
#define 	OSC_SYSCLK_Div32	0x50 
#define 	OSC_SYSCLK_Div64	0x60
#define 	OSC_SYSCLK_Div128	0x70   		

//外部振荡器方式
#define		EXT_OSC_DISABLE	 	0x00 	//外部振荡器电路禁止
#define		EXT_OSC_CMOS	 	0x20 	//外部CMOS时钟方式
#define		EXT_OSC_CMOS_Div2 	0x30 	//外部CMOS时钟方式二分频
#define		EXT_OSC_RC		 	0x40 	//外部RC振荡器方式
#define		EXT_OSC_C		 	0x50 	//外部电容振荡器方式
#define		EXT_OSC_CRY	 		0x60 	//外部晶体振荡器方式
#define		EXT_OSC_CRY_Div2	0x70 	//外部晶体振荡器方式二分频
//外部振荡器频率控制XFCN 							   晶体频率					（RC和 C方式） 
#define		EXT_OSC_FREQUENCY_RANGE0	0x00 	//	   f ≤ 20kHz 			    f ≤ 25 kHz 
#define		EXT_OSC_FREQUENCY_RANGE1	0x01 	//20 kHz < f ≤ 58 kHz		25 kHz < f ≤ 50 kHz 
#define		EXT_OSC_FREQUENCY_RANGE2 	0x02 	//58 kHz < f ≤ 155 kHz		50 kHz < f ≤ 100 kHz 
#define		EXT_OSC_FREQUENCY_RANGE3	0x03 	//155 kHz < f ≤ 415 kHz		100 kHz < f ≤ 200 kHz 
#define		EXT_OSC_FREQUENCY_RANGE4	0x04 	//415 kHz < f ≤ 1.1 MHz		200 kHz < f ≤ 400 kHz 
#define		EXT_OSC_FREQUENCY_RANGE5	0x05 	//1.1 MHz < f ≤ 3.1 MHz		400 kHz < f ≤ 800 kHz 
#define		EXT_OSC_FREQUENCY_RANGE6	0x06 	//3.1 MHz < f ≤ 8.2 MHz		800 kHz < f ≤ 1.6 MHz 
#define		EXT_OSC_FREQUENCY_RANGE7	0x07 	//8.2 MHz < f ≤ 25 MHz		1.6 MHz < f ≤ 3.2 MHz

/* 具体函数声明 */
extern void CloseWT(void); 							//系统配置初始化: 关闭看门狗
extern void SysClkInit(uint8 ClkType, uint8 ClkDIV, uint8 Xoscmd, uint8 Freq);  //系统时钟初始化

#endif
