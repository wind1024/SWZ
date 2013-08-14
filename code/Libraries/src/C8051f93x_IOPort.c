/**************************************************************************************************
** 文 件 名：C8051f93x_IOPort.c
** 作    者: xzy    版本: v01     编写日期: 2013    完成日期: 2013
** 功能描述:1. IO口初始化程序。
**           2. 使用宏定义函数IO_PORT_PIN_MODE(port, pin, mode,outmode)初始化某一引脚工作模式
**           3. 使用宏定义函数IO_PORT_MODE(port, mode,outmode)初始化某一端口工作模式
**           4. 使用宏定义函数PortUART0En()等使能数字外设的交叉开关
**           5. 只有P1口(ADC2)能初始化为模拟输入模式
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
#include 	"C8051f93x_IOPort.h"

/******************************************************************************
** 函 数 名: IOPortInit
** 功能描述: IO口初始化
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013-04-25
** 完成日期: 2013-04-25
******************************************************************************/ 
void IOPortInit(void)
{
	// vref agnd xtal1 xtal2 交叉开关跳过P0.0~P0.3
	IO_PORT_PIN_SKIP(0,0);
	IO_PORT_PIN_SKIP(0,1);
	IO_PORT_PIN_SKIP(0,2);
	IO_PORT_PIN_SKIP(0,3);
	// P0.0~P0.3配置为模拟方式
	IO_PORT_PIN_MODE(0,0,ANALOG_INPUT,NULL);
	IO_PORT_PIN_MODE(0,1,ANALOG_INPUT,NULL);
	IO_PORT_PIN_MODE(0,2,ANALOG_INPUT,NULL);
	IO_PORT_PIN_MODE(0,3,ANALOG_INPUT,NULL);
	// SDA和 SCL 连到端口引脚
	IO_PORT_PIN_SKIP(0,6);
	IO_PORT_PIN_SKIP(0,7);
	IO_PORT_PIN_MODE(0,6,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(0,7,DIGITAL_INPUT,PUSH_PULL_OUT);
	//PortSMB0En();
	//TX0 和RX0 连到端口引脚 P0.4和P0.5
	PortUART0En();
	// Set UART0 TX pin to push-pull 
	IO_PORT_PIN_MODE(0,4,DIGITAL_INPUT,PUSH_PULL_OUT);
	// 交叉开关跳过P1.0~P1.3、P1.6、P1.7
	IO_PORT_PIN_SKIP(1,0);
	IO_PORT_PIN_SKIP(1,1);
	IO_PORT_PIN_SKIP(1,2);
	IO_PORT_PIN_SKIP(1,3);
	//IO_PORT_PIN_SKIP(1,5);
	IO_PORT_PIN_SKIP(1,6);
	IO_PORT_PIN_SKIP(1,7);
	// p1.0 - p1.1 ad pin 模拟方式
	IO_PORT_PIN_MODE(1,0,ANALOG_INPUT,NULL);
	IO_PORT_PIN_MODE(1,1,ANALOG_INPUT,NULL);
	//P1.2,P1.3,P1.6 ,P1.7 推挽输出
	IO_PORT_PIN_MODE(1,2,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(1,3,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(1,6,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(1,7,DIGITAL_INPUT,PUSH_PULL_OUT);
	//IO_PORT_PIN_MODE(1,2,ANALOG_INPUT,NULL);
	//IO_PORT_PIN_MODE(1,3,ANALOG_INPUT,NULL);	
	// cex0 连到端口引脚P1.4
	PortPCA0M0En();
	// Set SOFT UART TX pin to push-pull 
	IO_PORT_PIN_MODE(1,5,DIGITAL_INPUT,PUSH_PULL_OUT);
	//PortCP1En();
	// P2.0~P2.6 推挽输出
	IO_PORT_PIN_MODE(2,0,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,1,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,2,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,3,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,4,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,5,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,6,DIGITAL_INPUT,PUSH_PULL_OUT);
	
	GlobalWeakPullEn();						//使能引脚的全局弱上拉功能
	CrossBarEn();							//交叉开关使能

	//不管交叉开关是否将一个端口引脚分配给某个数字外设，端口引脚的输出方式都受PnMDOUT寄存器控制。

}
