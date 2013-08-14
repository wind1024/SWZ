/**************************************************************************************************
** 文 件 名：C8051f93x_IOPort.h
** 作    者: xzy    版本: v01     编写日期: 2013    完成日期: 2013
** 功能描述:          
**---------------------------------------------------------------------------------------------------
** 硬件平台: 																					 
** 软件平台: 
**---------------------------------------------------------------------------------------------------
** 修改记录: 
** 			 
*****************************************************************************************************/
#ifndef 	C8051f93x_IOPORT_H
#define 	C8051f93x_IOPORT_H

#include "compiler_defs.h"             		// compiler declarations
#include "C8051F930_defs.h" 				// SFR declarations
#include "Compile.h"	

#define NULL    0
/* 端口测试时将其打开,测试后应关闭(把该条语句屏蔽) */

//端口的引脚号定义
#define		PIN_BIT0				0x01 			//PINx.0
#define		PIN_BIT1				0x02			//PINx.1
#define		PIN_BIT2				0x04			//PINx.2
#define		PIN_BIT3				0x08			//PINx.3
#define		PIN_BIT4				0x10			//PINx.4
#define		PIN_BIT5				0x20			//PINx.5
#define		PIN_BIT6				0x40			//PINx.6
#define		PIN_BIT7				0x80			//PINx.7

//引脚方式配置选项

#define 	PUSH_PULL_OUT   	1					//引脚输出推挽
#define		OPEN_DRAIN_OUT		2					//引脚输出开漏

//通过设置输出方式为"漏极开路"并向端口数据寄存器中的相应位写'1'将端口引脚配置为数字输入 
#define		DIGITAL_INPUT		3					//引脚数字输入

//只有P1口可以被配置为模拟输入(ADC2)，因为只有P1口有P1MDIN寄存器
//被配置为模拟输入的引脚的输出驱动器并没有被明确地禁止。
//因此被配置为模拟输入的引脚所对应的P1MDOUT 位应被明确地设置为逻辑'0'（漏极开路方式）,
//对应的端口数据位应被设置为逻辑'1'（高阻态）
//禁止被配置为模拟输入的引脚的输出驱动器
#define		ANALOG_INPUT		4					//引脚模拟输入(只有P1口可以)
//外部中断引脚分配（引脚分配与交叉开关无关）
#define INT0_PORT_PIN(pin)		IT01CF |= (pin)
#define INT1_PORT_PIN(pin)		IT01CF |= (pin << 4)
//交叉开关跳过引脚
#define IO_PORT_PIN_SKIP(port, pin)		P##port##SKIP |= (0x01 << (pin))

//mode = {PUSH_PULL_OUT, OPEN_DRAIN_OUT, DIGITAL_INPUT, ANALOG_INPUT}
//port = {0, ..., 7},    pin  = {0, ..., 7}
#define IO_PORT_PIN_MODE(port, pin, mode,outmode)  	\
do{                                  		\
	if (mode == DIGITAL_INPUT){            	\
		P##port##MDIN |= (0x01 << (pin)); \
		P##port        |= (0x01 << (pin));  \
		if (outmode == PUSH_PULL_OUT)     	\
	 		P##port##MDOUT |= (0x01 << (pin));  \
		else if(outmode == OPEN_DRAIN_OUT)      \
	 		P##port##MDOUT &= ~(0x01 << (pin)); \
	}										\
	else if(mode == ANALOG_INPUT) {			\
		P##port##MDIN  &= ~(0x01 << (pin)); \		
		P##port##MDOUT &= ~(0x01 << (pin));	\
		P##port##      |= (0x01 << (pin)); 	\
	}										\
}while(0)

//mode = {PUSH_PULL_OUT, OPEN_DRAIN_OUT, IGITAL_INPUT, ANALOG_INPUT}
//port = {0, ..., 7}
#define IO_PORT_MODE(port, mode,outmode)  	\
do{                                  		\
  	if(mode == DIGITAL_INPUT){				\
		P##port##MDIN = 0xff;				\
		P##port        |= 0xff;  			\
  		if (outmode == PUSH_PULL_OUT)          	\
     		P##port##MDOUT |= 0xff;  			\
  		else if(outmode == OPEN_DRAIN_OUT)     	\
     		P##port##MDOUT &= 0x00; 			\
	}										\
	else if(mode == ANALOG_INPUT) {			\
		P##port##MDIN  &= 0x00; 			\		
		P##port##MDOUT &= 0x00;				\
		P##port##      |= 0xff; 			\
	}										\
}while(0)



//数字外设的交叉开关使能位定义(按优先级由高到低排序)
/* 1. 如果将EMIF选到低端口(文件"EMIF.h"中: #define XRAM_PORT_HIGH_EN 0),
      且使用总线非复用方式(文件"EMIF.h"中: #define	XRAM_MULTIPLEX_EN 0),
      则P1.6用作/RD,P1.7用作/WR,P1口用作非复用地址高,P2口用作非复用地址低,P3口用作数据总线

   2. 如果将EMIF选到低端口(文件"EMIF.h"中: #define XRAM_PORT_HIGH_EN 0),
      且使用总线复用方式(文件"EMIF.h"中: #define XRAM_MULTIPLEX_EN 1),
      则P1.5用作ALE,P1.6用作/RD,P1.7用作/WR,P2口用作复用地址高,P3口用作复用地址低 和 数据总线
   
   3. 如果P1口的全部或某几位用作ADC2,则P1全部或某几位不能用作交叉开关
*/
#define		PORT_UART0_EN			0x01			//XBR0.0(使用2 个引脚TX0(P0.4),RX0(P0.5))
//当SPI 被设置为3 线方式时，NSS 信号不被分配端口引脚
#define		PORT_SPI1_EN			0x40			//XBR1.6(使用3 个或4 个引脚SCK,MISO,MOSI,NSS)从P1.0开始
#define		PORT_SPI0_EN			0x02			//XBR0.1(使用3 个或4 个引脚SCK,MISO,MOSI,NSS)从P0.0开始
#define		PORT_SMB0_EN			0x04			//XBR0.2(使用2 个引脚SDA,SCL)
#define		PORT_CP0_EN				0x10			//XBR0.4(使用1 个引脚CP0)
#define		PORT_CP0A_EN			0x20			//XBR0.5(使用1 个引脚CP0A)
#define		PORT_CP1_EN				0x40			//XBR0.6(使用1 个引脚CP1)
#define		PORT_CP1A_EN			0x80			//XBR0.7(使用1 个引脚CP1A)
#define		PORT_SYSCLK_EN			0x08			//XBR0.3(使用1 个引脚SYSCLK)
//XBR1.[2:0](使用6 个引脚CEX0,CEX1,CEX2,CEX3,CEX4,CEX5)
#define		PORT_PCA0M0_EN			0x01			//使用1 个引脚CEX0			
#define		PORT_PCA0M0_1_EN		0x02			//使用2 个引脚CEX0,CEX1
#define		PORT_PCA0M0_2_EN		0x03			//使用3 个引脚CEX0,CEX1,CEX2
#define		PORT_PCA0M0_3_EN		0x04			//使用4 个引脚CEX0,CEX1,CEX2,CEX3
#define		PORT_PCA0M0_4_EN		0x05			//使用5 个引脚CEX0,CEX1,CEX2,CEX3,CEX4
#define		PORT_PCA0M0_5_EN		0x06			//使用6 个引脚CEX0,CEX1,CEX2,CEX3,CEX4,CEX5
#define		PORT_ECI_EN				0x08			//XBR1.3(使用1 个引脚ECI)
#define		PORT_T0_EN				0x10			//XBR1.4(使用1 个引脚T0) 
#define		PORT_T1_EN				0x20			//XBR1.5(使用1 个引脚T1)
	
//使用2 个引脚TX0,RX0
#define		PortUART0En()	XBR0 |= PORT_UART0_EN		

//使用3 个或4 个引脚SCK,MISO,MOSI,NSS
#define		PortSPI1En()	XBR1 |= PORT_SPI1_EN									
#define		PortSPI0En()	XBR0 |= PORT_SPI0_EN		

//使用2 个引脚SDA,SCL		
#define		PortSMB0En()	XBR0 |= PORT_SMB0_EN

//使用1 个引脚CP0	 	
#define		PortCP0En()		XBR0 |= PORT_CP0_EN		

//使用1 个引脚CP0A	 	
#define		PortCP0AEn()	XBR0 |= PORT_CP0A_EN

//使用1 个引脚CP1		
#define		PortCP1En()		XBR0 |= PORT_CP1_EN	

//使用1 个引脚CP1A		
#define		PortCP1AEn()	XBR0 |= PORT_CP1A_EN

//使用1 个引脚SYSCLK		
#define		PortSYSCLKEn()	XBR0 |= PORT_SYSCLK_EN

//使用1 个引脚CEX0		
#define		PortPCA0M0En()	XBR1 = (XBR1 & (~0x07)) | PORT_PCA0M0_EN

//使用2 个引脚CEX0,CEX1	
#define		PortPCA0M0_1En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_1_EN

//使用3 个引脚CEX0,CEX1,CEX2
#define		PortPCA0M0_2En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_2_EN	

//使用4 个引脚CEX0,CEX1,CEX2,CEX3	
#define		PortPCA0M0_3En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_3_EN

//使用5 个引脚CEX0,CEX1,CEX2,CEX3,CEX4
#define		PortPCA0M0_4En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_4_EN

//使用6 个引脚CEX0,CEX1,CEX2,CEX3,CEX4,CEX5
#define		PortPCA0M0_5En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_5_EN

//使用1 个引脚ECI
#define		PortECI0En()	XBR1 |= PORT_ECI0_EN

//使用1 个引脚T0		
#define		PortT0En()	XBR1 |= PORT_T0_EN

//使用1 个引脚T1		
#define		PortT1En()	XBR1 |= PORT_T1_EN

//使能引脚的全局弱上拉功能
#define		GlobalWeakPullEn()				\			
do{                                  		\
  	XBR2   &= ~0x80;						\
}while(0)

//禁止引脚的全局弱上拉功能
#define		GlobalWeakPullDisable()			\			
do{                                  		\
  	XBR2   |= 0x80;							\
}while(0)

//交叉开关使能
#define		CrossBarEn()					\			
do{                                  		\
  	XBR2    |= 0x40;						\
}while(0)

//交叉开关禁能
#define		CrossBarDisable()				\			
do{                                  		\
  	XBR2    &= ~0x40;						\
}while(0)


/* 具体函数声明 */
extern void IOPortInit(void); 						//IO口初始化


#endif
