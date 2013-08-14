/**************************************************************************************************
** 文 件 名：C8051f93x_SMBus.h
** 作    者: xzy    版本: v01     编写日期: 2013.05    完成日期: 2013
** 功能描述:                       
**---------------------------------------------------------------------------------------------------
** 硬件平台: 
**           ML-EC5仿真器;        																					 
** 软件平台: KEIL4.10
**---------------------------------------------------------------------------------------------------
** 修改记录: 
** 			 
*****************************************************************************************************/

#ifndef C8051f93x_SMBUS_H
#define C8051f93x_SMBUS_H

#include "compiler_defs.h"             		// compiler declarations
#include "C8051F930_defs.h" 				// SFR declarations
#include "Compile.h"
#include "C8051f93x_SysClk.h"
#include "user_type.h"

#define		SMBUS_GENERATE_TIMER	3		//选用定时器2低字节溢出作为SMBus时钟源

#define		SMBus_INTERRUPT_EN		0

#define  SMB_FREQUENCY  100000          // SCL时钟速率


#define  WRITE          0x00           // SMBus 写控制
#define  READ           0x01           // SMBus 读控制
                                   
// 状态向量 :SMB0CN 高 4 位
#define  SMB_MTSTA      0xE0           // (MT) 开始发送
#define  SMB_MTDB       0xC0           // (MT) 数据字节发送
#define  SMB_MRDB       0x80           // (MR) 数据接收

//SMB0CF：SMBus 配置寄存器

//SMBus 使能控制 
#define SMBusEn()	SMB0CF |= 0x80
#define SMBusDis()	SMB0CF &= ~0x80
//SMBus 从方式控制 
#define SMBusSlaveModeDis()	SMB0CF |= 0x40
#define SMBusSlaveModeEn()	SMB0CF &= ~0x40

/* 具体函数声明 */
extern void SMBusInit(void); 						//ISMBus初始化
//向有子地址器件发送多字节数据函数
extern Bool SMBusSendStr(Uint8 sla,Uint16 suba,Uint8 * s,Uint8 no,Uint8 p)reentrant;
//向有子地址器件读取多字节数据函数
extern Bool SMBusRcvStr(Uint8 sla,Uint16 suba,Uint8 * s,Uint8 no,Uint8 p)reentrant;


#endif