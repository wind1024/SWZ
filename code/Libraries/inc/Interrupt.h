/*
*************************************************************************************************
** 文 件 名：Interrupt.h
** 作    者: xzy    版本: v01     编写日期: 2013    完成日期: 2013
** 功能描述: 中断向量，及中断控制                       
**---------------------------------------------------------------------------------------------------
** 硬件平台: 
**           ML-EC5仿真器;        																					 
** 软件平台: KEIL4.10
**---------------------------------------------------------------------------------------------------
** 修改记录: 
** 			 
****************************************************************************************************
*/
#ifndef 	_INTERRUPT_H
#define 	_INTERRUPT_H

#include <compiler_defs.h>             		// compiler declarations
#include <C8051F930_defs.h> 				// SFR declarations
//#include "Compile.h"	

//优先级顺序定义（见C8051F9xx 中文版.pdf的P111）
//复位中断：中断向量地址0x0000; 中断标志位 (无); 中断使能位(一直使能); 中断优先级控制位(总是最高) 
//#define	RST_PRIOR_ORDER			//最高 

//外部中断0：中断向量地址0x0003; 中断标志位IE0(TCON.1); 中断使能位EX0(IE.0); 中断优先级控制位PX0(IP.0) 
#define		EINT0_PRIOR_ORDER		0  	

//定时器中断0：中断向量地址0x000B; 中断标志位TF0(TCON.5); 中断使能位ET0(IE.1); 中断优先级控制位PT0(IP.1)
#define		TIMER0_PRIOR_ORDER		1

//外部中断1：中断向量地址0x0013; 中断标志位IE1 (TCON.3); 中断使能位EX1 (IE.2); 中断优先级控制位PX1 (IP.2) 
#define		EINT1_PRIOR_ORDER		2  	

//定时器中断1：中断向量地址0x001B; 中断标志位TF1 (TCON.7); 中断使能位ET1 (IE.3); 中断优先级控制位PT1 (IP.3)
#define		TIMER1_PRIOR_ORDER		3

//串行口中断0：中断向量地址0x0023; 中断标志位RI0(SCON0.0), TI0(SCON0.1); 中断使能位ES0(IE.4); 中断优先级控制位PS0(IP.4)
#define		UART0_PRIOR_ORDER		4  	

//定时器中断2：中断向量地址0x002B; 中断标志位TF2(TMR2CN.7), EXF2(TMR2CN.6); 中断使能位ET2(IE.5); 中断优先级控制位PT2(IP.5)
#define		TIMER2_PRIOR_ORDER		5

//SPI0中断：中断向量地址0x0033; 中断标志位SPIF(SPI0CN.7), WCOL(SPI0CN.6), MODF(SPI0CN.5), RXOVRN(SPI0CN.4); 
//         中断使能位ESPI0(IE.6); 中断优先级控制位PSPI0(IP.6) 
#define		SPI0_PRIOR_ORDER		6  	

//SMBus中断：中断向量地址0x003B; 中断标志位SI(SMB0CN.0); 中断使能位ESMB0(EIE1.0); 中断优先级控制位PSMB0(EIP1.0)
#define		SMBUS_PRIOR_ORDER		7

//smaRTClock告警中断：中断向量地址0x0043; 中断标志位ALRM(RTC0CN.2); 中断使能位ERTC0A(EIE1.1); 中断优先级控制位PRTC0A(EIP1.1)
#define		RTC0_ALARM_PRIOR_ORDER	8   

//ADC0窗口比较中断：中断向量地址0x004B; 中断标志位AD0WINT(ADC0CN.3); 中断使能位EWADC0(EIE1.2); 中断优先级控制位PWADC0(EIP1.2) 
#define		ADC0_WINDOW_CMP_PRIOR_ORDER		9  	

//ADC0转换结束中断：中断向量地址0x0053; 中断标志位AD0INT(ADC0CN.5); 中断使能位EADC0(EIE1.3); 中断优先级控制位PADC0(EIP1.3) 
#define		ADC0_END_CONVERT_PRIOR_ORDER	10  	

//PCA0中断：中断向量地址0x005B; 中断标志位CF(PCA0CN.7), CCFn(PCA0CN.n); 中断使能位EPCA0(EIE1.4); 中断优先级控制位PPCA0(EIP1.4)
#define		PCA0_PRIOR_ORDER		11

//比较器0中断：中断向量地址0x0063; 中断标志位CP0FIF(CPT0CN.4),CP0FIF(CPT0CN.5); 中断使能位ECP0(EIE1.5); 中断优先级控制位PCP0(EIP1.5) 
#define		COMPARATOR0_PRIOR_ORDER		12  		

//比较器1中断：中断向量地址0x006B; 中断标志位CP1FIF(CPT1CN.4),CP1FIF(CPT1CN.5); 中断使能位ECP1(EIE1.6); 中断优先级控制位PCP1(EIP1.6) 
#define		COMPARATOR1_PRIOR_ORDER		13  	

//定时器3中断：中断向量地址0x0073; 中断标志位TF3H(TMR3CN.7),TF3L(TMR3CN.6); 中断使能位ET3(EIE1.7); 中断优先级控制位PT3(EIP1.7) 
#define		TIMER3_PRIOR_ORDER			14  	

//VDD/DC+电源监视器早期告警中断：中断向量地址0x007B; 中断标志位VDDOK(VDM0CN.5); 中断使能位EWARN(EIE2.0); 中断优先级控制位PWARN(EIP2.0) 
#define		VDD_WARNING_PRIOR_ORDER		15  	

//端口匹配中断：中断向量地址0x0083; 中断标志位 无; 中断使能位EMAT(EIE2.1); 中断优先级控制位PMAT(EIP2.1)
#define		PORT_MATCH_PRIOR_ORDER		16

//smaRTClock 振荡器故障中断：中断向量地址0x008B; 中断标志位OSCFAIL(RTC0CN.5); 中断使能位ERTC0F(EIE2.2); 中断优先级控制位PRTC0F(EIP2.2)
#define		RTC0_OSC_FAIL_PRIOR_ORDER	17

//SPI1中断：中断向量地址0x0093; 中断标志位SPIF(SPI1CN.7), WCOL(SPI1CN.6), MODF(SPI1CN.5), RXOVRN(SPI1CN.4); 
//         中断使能位ESPI1(EIE2.3); 中断优先级控制位PSPI1(EIP2.3) 
#define		SPI1_PRIOR_ORDER		18  	



/* 宏定义函数定义 */
//中断允许寄存器IE可在所有页访问
//扩展中断允许寄存器1EIE1可在所有页访问
//扩展中断允许寄存器1EIE2可在所有页访问
//开启中断
/* IE寄存器 */
#define	GlobleIntSwitchOn()		EA   = 1 			//全局中断开关 打开
#define	SPI0IntSwitchOn()		ESPI0 = 1			//允许串行外设接口0（SPI0）中断
#define	Timer2IntSwitchOn()		ET2  = 1			//定时器2中断开
#define	UART0IntSwitchOn()		ES0  = 1			//UART0中断开	
#define	Timer1IntSwitchOn()		ET1  = 1 			//定时器1中断开
#define	EINT1IntSwitchOn()		EX1  = 1			//外部中断1中断开
#define	Timer0IntSwitchOn()		ET0  = 1			//定时器0中断开	
#define	EINT0IntSwitchOn()		EX0  = 1			//外部中断0中断开

/* EIE1 寄存器 */
#define Timer3IntSwitchOn()		EIE1 |= (0x01 << 7) //定时器3中断开	
#define	CP1IntSwitchOn()		EIE1 |= (0x01 << 6) //允许比较器1（CP1）中断
#define	CP0IntSwitchOn()		EIE1 |= (0x01 << 5) //允许比较器0（CP0）中断	
#define	PCA0IntSwitchOn()		EIE1 |= (0x01 << 4) //允许可编程计数器阵列（PCA0）中断
#define	ADC0IntSwitchOn()		EIE1 |= (0x01 << 3) //允许ADC0 转换结束中断
#define	WADC0IntSwitchOn()		EIE1 |= (0x01 << 2) //允许ADC0 窗口比较中断
#define	RTC0AIntSwitchOn()		EIE1 |= (0x01 << 1)	//允许smaRTClock告警中断	
#define	SMB0IntSwitchOn()		EIE1 |= 0x01		//允许SMBus0 中断


/* EIE2 寄存器 */
#define	SPI1IntSwitchOn()		EIE2 |= (0x01 << 3) //允许串行外设接口1（SPI1）中断
#define	RTC0FIntSwitchOn()		EIE2 |= (0x01 << 2) //允许smaRTClock 振荡器故障中断
#define	MATIntSwitchOn()		EIE2 |= (0x01 << 1) //允许端口匹配中断
#define	WARNIntSwitchOn()		EIE2 |= 0x01 		//允许VDD/DC+电源监视器早期告警中断
		
//关闭中断
#define	GlobleIntSwitchOff()	EA   = 0 			//全局中断开关 关闭
#define	SPI0IntSwitchOff()		ESPI0  = 0			//串行外设接口0（SPI0）中断关闭
#define	Timer2IntSwitchOff()	ET2  = 0			//定时器2中断关闭
#define	UART0IntSwitchOff()		ES0  = 0			//UART0中断关闭	
#define	Timer1IntSwitchOff()	ET1  = 0 			//定时器1中断关闭
#define	EINT1IntSwitchOff()		EX1  = 0			//外部中断1中断关闭
#define	Timer0IntSwitchOff()	ET0  = 0			//定时器0中断关闭	
#define	EINT0IntSwitchOff()		EX0  = 0			//外部中断0中断关闭

#define	Timer3IntSwitchOff()	EIE1 &= (~(0x01 << 7)) //定时器3中断关闭
#define	CP1FIntSwitchOff()		EIE1 &= (~(0x01 << 6)) //比较器1（CP1）中断关闭
#define	CP0RIntSwitchOff()		EIE1 &= (~(0x01 << 5)) //比较器0（CP0）中断关闭	
#define	PCA0IntSwitchOff()		EIE1 &= (~(0x01 << 4)) //可编程计数器阵列（PCA0）中断关闭
#define	ADC0IntSwitchOff()		EIE1 &= (~(0x01 << 3)) //ADC0 转换结束中断关闭
#define	WADC0IntSwitchOff()		EIE1 &= (~(0x01 << 2)) //ADC0 窗口比较中断关闭	
#define	RTC0AIntSwitchOFF()		EIE1 &= (~(0x01 << 1)) //smaRTClock告警中断关闭
#define	SMB0IntSwitchOff()		EIE1 &= ~0x01		 //SMBus0 中断关闭

#define	SPI1IntSwitchOff()		EIE2 &= (~(0x01 << 3)) //串行外设接口1（SPI1）中断关闭	
#define	RTC0FIntSwitchOff()		EIE2 &= (~(0x01 << 2)) //smaRTClock 振荡器故障中断关闭
#define	MATIntSwitchOff()		EIE2 &= (~(0x01 << 1)) //端口匹配中断关闭
#define	WARNIntSwitchOff()		EIE2 &= ~0x01 		 //VDD/DC+电源监视器早期告警中断关闭


//中断优先级寄存器IP可在所有页访问
//扩展中断优先级寄存器EIP1可在所有页访问
//扩展中断优先级寄存器EIP2可在所有页访问
/* IP寄存器 */
#define SPI0IntPriorH()			PSPI0 = 1			//串行外设接口（SPI0）中断优先级高
#define	Timer2IntPriorH()		PT2  = 1			//定时器2中断优先级高
#define	UART0IntPriorH()		PS0  = 1			//UART0中断优先级高	
#define	Timer1IntPriorH()		PT1  = 1 			//定时器1中断优先级高
#define	EINT1IntPriorH()		PX1  = 1			//外部中断1中断优先级高
#define	Timer0IntPriorH()		PT0  = 1			//定时器0中断优先级高	
#define	EINT0IntPriorH()		PX0  = 1			//外部中断0中断优先级高

/* EIP1 寄存器 */
#define	Timer3IntPriorH()		EIP1 |= (0x01 << 7) //允许定时器3 中断优先级高
#define	CP1IntPriorH()			EIP1 |= (0x01 << 6) //允许比较器1（CP1）中断优先级高
#define	CP0IntPriorH()			EIP1 |= (0x01 << 5) //允许比较器0（CP0）中断优先级高	
#define	PCA0IntPriorH()			EIP1 |= (0x01 << 4) //允许可编程计数器阵列（PCA0）中断优先级高
#define	ADC2IntPriorH()			EIP1 |= (0x01 << 3) //允许ADC2 转换结束中断优先级高
#define	WADC0IntPriorH()		EIP1 |= (0x01 << 2) //允许ADC0 窗口比较中断优先级高	
#define	RTC0AIntPriorH()		EIP1 |= (0x01 << 1) //允许smaRTClock告警中断优先级高
#define	SMB0IntPriorH()			EIP1 |= 0x01		//允许SMBus0 中断优先级高

/* EIP2 寄存器 */
#define	SPI1IntPriorH()			EIP2 |= (0x01 << 3) //允许串行外设接口（SPI1）中断优先级高
#define	RTC0FIntPriorH()		EIP2 |= (0x01 << 2) //允许smaRTClock 振荡器故障中断优先级高
#define	MATIntPriorH()			EIP2 |= (0x01 << 1) //允许端口匹配中断优先级高
#define	WARNIntPriorH()			EIP2 |= 0x01 		 //允许VDD/DC+电源监视器早期告警中断优先级高

//设置为低优先级
/* IP寄存器 */
#define SPI0IntPriorL()			PSPI0 = 0			//串行外设接口（SPI0）中断优先级低
#define	Timer2IntPriorL()		PT2  = 0			//定时器2中断优先级低
#define	UART0IntPriorL()		PS0  = 0			//UART0中断优先级低
#define	Timer1IntPriorL()		PT1  = 0 			//定时器1中断优先级低
#define	EINT1IntPriorL()		PX1  = 0			//外部中断1中断优先级低
#define	Timer0IntPriorL()		PT0  = 0			//定时器0中断优先级低	
#define	EINT0IntPriorL()		PX0  = 0			//外部中断0中断优先级低

/* EIP1 寄存器 */
#define	Timer3IntPriorL()		EIP1 &= (~(0x01 << 7)) //允许定时器3 中断优先级低
#define	CP1IntPriorL()			EIP1 &= (~(0x01 << 6)) //允许比较器1（CP1）中断优先级低
#define	CP0IntPriorL()			EIP1 &= (~(0x01 << 5)) //允许比较器0（CP0）中断优先级低	
#define	PCA0IntPriorL()			EIP1 &= (~(0x01 << 4)) //允许可编程计数器阵列（PCA0）中断优先级低
#define	ADC2IntPriorL()			EIP1 &= (~(0x01 << 3)) //允许ADC2 转换结束中断优先级低
#define	WADC0IntPriorL()		EIP1 &= (~(0x01 << 2)) //允许ADC0 窗口比较中断优先级低	
#define	RTC0AIntPriorL()		EIP1 &= (~(0x01 << 1)) //允许smaRTClock告警中断优先级低
#define	SMB0IntPriorL()			EIP1 &= ~0x01		//允许SMBus0 中断优先级低

/* EIP2 寄存器 */
#define	SPI1IntPriorL()			EIP2 &= (~(0x01 << 3)) //允许串行外设接口（SPI1）中断优先级低
#define	RTC0FIntPriorL()		EIP2 &= (~(0x01 << 2)) //允许smaRTClock 振荡器故障中断优先级低
#define	MATIntPriorL()			EIP2 &= (~(0x01 << 1)) //允许端口匹配中断优先级低
#define	WARNIntPriorL()			EIP2 &= ~0x01 		 //允许VDD/DC+电源监视器早期告警中断优先级低
// TCON 定时器控制寄存器 
#define EINT0LevelTrigger()		TCON &= ~0x01		//外部中断0电平触发
#define EINT1LevelTrigger()		TCON &= (~(0x01 << 2))//外部中断1电平触发

#define EINT0EdgeTrigger()		TCON |= 0x01		//外部中断0边沿触发
#define EINT1EdgelTrigger()		TCON |= (0x01 << 2)	//外部中断1边沿触发

//TMRnCN.5定时器低字节中断控制
#define TIMER2LENIntON()		TF2LEN = 1			//定时器2 低字节中断允许
#define TIMER3LENIntON()		TMR3CN |= (0x01 << 5)	//定时器3 低字节中断允许

#define TIMER2LENIntOFF()		TF2LEN = 0			//定时器2 低字节中断禁止
#define TIMER3LENIntOFF()		TMR3CN &= (~(0x01 << 5))	//定时器3 低字节中断禁止


#endif