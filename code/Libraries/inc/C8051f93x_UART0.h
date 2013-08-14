/*
*************************************************************************************************
** 文 件 名：C8051f93x_UART0.h
** 作    者: xzy    版本: v01     编写日期: 2013.04.27    完成日期: 2013
** 功能描述:          
**---------------------------------------------------------------------------------------------------
** 硬件平台: 
**           ML-EC5仿真器;        																					 
** 软件平台: KEIL4.10
**---------------------------------------------------------------------------------------------------
** 修改记录: 
** 			 
****************************************************************************************************
*/
#ifndef 	C8051f93x_UART0_H
#define 	C8051f93x_UART0_H

#include "compiler_defs.h"             		// compiler declarations
#include "C8051F930_defs.h" 				// SFR declarations
#include "Compile.h"
#include "C8051f93x_SysClk.h"	
#include "C8051f93x_Timer01.h"

#define		BAUD_GENERATE_TIMER1	1		//选用定时器1作为波特率发生器

#define		UART0_INTERRUPT_EN		1       //中断服务控制

#if	UART0_INTERRUPT_EN > 0
//extern	uint8	UART0RecvBuf;
//extern	uint8	UART0RecvIntFlag;
#endif

/* 波特率宏定义 */

#define 	BAUDRATE           115200         // 波特率 >=1200	最高测试过115200

//#define		UART0_BAUD_DOUBLE		0			//是否需要波特率加倍

/* 串口方式宏定义 */
#define		UART0_MODE1				0			//方式1：8 位UART，可变波特率
#define		UART0_MODE3				1			//方式3：9 位UART，可变波特率
//多处理器通信控制
#define		UART0MCEEN			MCE0 = 1 
#define		UART0MCEDIS			MCE0 = 0
//接收控制
//#define		UART0RECVEN			 REN0 = 1
//#define		UART0RECVDIS		 REN0 = 0

/* 具体函数声明 */
void UART0Init(uint8 UART0Mode);
//extern void UART0SendByte(uint8 Val);			//通过UART0发送一个字节
//extern void UART0_ReadyRecv(void);				//UART0准备接收新字节
//extern uint8 UART0_RecvNewDat(void);			//是否有新数据
//extern uint8 UART0_RecvOneDat(void);			//接收一个字节数据



#endif