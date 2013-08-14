/**************************************************************************************************
** 文 件 名：C8051f93x_ADC.c
** 作    者: xzy    版本: v01     编写日期: 2013    完成日期: 2013
** 功能描述: ADC0初始化相关函数                      
**---------------------------------------------------------------------------------------------------
** 硬件平台: 
**           ML-EC5仿真器;        																					 
** 软件平台: KEIL4.10
**---------------------------------------------------------------------------------------------------
** 修改记录: 
** 			 
*****************************************************************************************************/
#include "compile.h"
#include "C8051f93x_SysClk.h"
#include "C8051f93x_ADC.H"
#include "Interrupt.h"
#include "memory.h"

//unsigned int result[9];                // AIN0-7和温度传感器缓存

//extern SYSCLK;
extern SYS_Delay(unsigned int n);


/*****************************************************************************
AD初始化
******************************************************************************/
void ADC0_Init (void)
{
   //电压控制基准寄存器,位0 内部电压基准输出,位2 温度传感器,位4-3 电压基准选择,位5 模拟地基准选择。
   REF0CN = 0x24;						
   //ADC0 控制寄存器,位1-0启动方式选择，位6 BURSTEN突发模式使能，位7：ADC0 使能位 
   ADC0CN = 0x00;
   //DC0 配置寄存器,位0 增益控制，位1 跟踪方式，位2：ADC0 8位方式使能 ，位7-3转换时钟分频系数  
   ADC0CF = (SYSCLK/5000000) << 3;     // ADC转换时钟= 5MHz
   ADC0CF |= 0x03;
   ADC0AC = 0x00;
}
/******************************************************************************
** 函 数 名: void ADC_Over(void)
** 功能描述: 关闭ADC0
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.04
******************************************************************************/ 
void ADC_Over(void)
{
	ADC0CN = 0;
	REF0CN = 0;
	ADC0CF = 0;
}
/******************************************************************************
** 函 数 名: void ADC_enable(void) 
** 功能描述: 开启AD转换
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.04
******************************************************************************/ 
void ADC_enable(void)
{
	AD0EN = 1; 			// 使能ADC
	ADC0IntSwitchOn(); 	// 开启ADC中断
}
/******************************************************************************
** 函 数 名: void ADC_disable(void)
** 功能描述: 停止AD转换
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.04
******************************************************************************/ 
void ADC_disable(void)
{
	AD0EN = 0; 			//禁用ADC
	ADC0IntSwitchOff();	//AD中断关闭
}

/******************************************************************************
** 函 数 名: uint ADC0_Converx(unsigned char mux)
** 功能描述: AD任意一路转换  参数mux 代表第几路(0~8)
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.04
******************************************************************************/ 
void ADC0_Converx(unsigned char mux)
{
	//uint16 xdata temp;
	ADC0_Init();
	ADC0MX = mux;
	SysDat.channelADC = mux;
	//SYS_Delay(20);
	ADC_enable();
	AD0INT = 0;
	AD0BUSY = 1;
	while(SysDat.channelADC != 0xff);
	//ADC_disable();
}
/*
*****************************************************************************
** 函 数 名: INTERRUPT (ADC_ISR, INTERRUPT_ADC0_EOC)
** 功能描述: ADC转换结束中断
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/
INTERRUPT (ADC_ISR, INTERRUPT_ADC0_EOC)//ADC转换结束中断
{  	
	Uint16 xdata tempData;

	AD0BUSY	 = 0;
	while(!AD0INT);
	AD0INT 	 = 0;			 								// Clear the flag of ADC over
	tempData = ADC0;

	if(ADC0MX==SysDat.channelADC)
	{
		switch(SysDat.channelADC)
		{
		case LOAD:
			GatDat->TempLoad[SysDat.pFilter] = tempData;
			break;
		case VOLTAGE:
			GatDat->TempLoad[SysDat.pFilter] = tempData;
			break;
		case TEMPERATURE:
			GatDat->TempAcc[SysDat.pFilter++] = tempData;
			break;
		default:
			break;
		}
	}
	SysDat.channelADC = 0xff;										// End this channel ADC
	ADC_disable();	
} 
