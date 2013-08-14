/**************************************************************************************************
** 文 件 名：C8051f93x_ADC.H
** 作    者: xzy    版本: v01     编写日期: 2013    完成日期: 2013
** 功能描述: ADC0头文件                      
**---------------------------------------------------------------------------------------------------
** 硬件平台: 
**           ML-EC5仿真器;        																					 
** 软件平台: KEIL4.10
**---------------------------------------------------------------------------------------------------
** 修改记录: 
** 			 
*****************************************************************************************************/
/*
//REF0CN: 电压基准控制寄存器 
#define		ADC0_REFGND		0			//ADC0地基准选择 0：GND引脚，1：P0.1/AGND 引脚
 
#define		ADC0_REFSL0		0			//ADC0的电压基准为 P0.0/VREF引脚
#define		ADC0_REFSL1		1			//ADC0的电压基准为 VDD/DC+引脚。 
#define		ADC0_REFSL2		2			//ADC0的电压基准为内部 1.8V数字电源电压
#define		ADC0_REFSL3		3			//ADC0 的电压基准为内部 1.65V高速电压基准

#define		ADC0_TEMPE		0			//温度传感器控制 0：禁止，1：使能

#define		ADC0_INVREF		0			//内部电压基准输出控制 0：禁止，1：使能

//IREF0CN:  电流基准控制寄存器
#define		ADC0_SINK		0			//IREF0 灌电流控制 0：IREF0提供拉电流，1：IREF0 提供灌电流

#define		ADC0_MDSEL		0			//IREF0 输出方式控制 0:选择低功耗模式（步长 = 1 μA）,1:选择大电流模式（步长 = 8 μA) 

#define		ADC0_IREFDAT	0			//指定获得期望输出电流所需要的步数(0~63),输出电流 = 方向 ×  步长 × IREF0DAT
*/

//extern void ADC0_Init (void);
//extern void ADC0_Conver (void);
extern void ADC_Over(void);
extern void ADC0_Converx(unsigned char mux);
//extern char* ADResult(float result);