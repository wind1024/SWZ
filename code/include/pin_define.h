/***********************************************************************************
**文件说明：MCU管脚功能别名定义                                                            
**文件名称：pin.define.h                                                       
**创建日期：2013.                                                         
**创 建 人：XZY                                                                   
**---------------------------------------------------------------------------------*
**修改日期：                                                                       
**修改说明：                                                                       
***********************************************************************************/

#ifndef PIN_DEFINE
#define PIN_DEFINE

#include <compiler_defs.h>             		// compiler declarations
#include <C8051F930_defs.h> 				// SFR declarations



	
sbit SDA		= P0^6;			//模拟I2C数据传送位
sbit SCL		= P0^7;			//模拟I2C时钟控制位

sbit DATA_OK	= P1^2;			//加速度数据就绪
sbit CHARGE_C	= P1^3;			//充电控制

sbit SW_RX		= P1^4;  		// SW_UART Receive pin软件串口接收引脚
sbit SW_TX		= P1^5;  		// SW_UART Transmit pin软件串口发送引脚

sbit ZIGBEE_RST	= P1^6;			//ZigBee复位控制
sbit G_POWER	= P1^7;			//加速度模块电源

sbit PC_SET		= P2^0;			//参数设置 ,调试模式
sbit GSM_REST	= P2^1;			//GSM/GPRS模块复位
sbit GSM_RTS	= P2^2;			//GSM/GPRS模块RTS信号
sbit GSM_CTS	= P2^3;			//GSM/GPRS模块CTS信号
//sbit GSM_RESET	= P2^4;			//GSM/GPRS模块复位
sbit USER_LED	= P2^4;			//
sbit SYS_POWER	= P2^6;			//系统电源控制
#endif
