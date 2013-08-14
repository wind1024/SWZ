/***********************************************************************************
**文件说明：C8051f93x时钟系统配置                                                              
**文件名称：C8051f93x_SysClk.c                                                      
**创建日期：2013.04                                                            
**创 建 人：XZY                                                                   
**---------------------------------------------------------------------------------*
**修改日期：                                                                       
**修改说明：                                                                       
***********************************************************************************/

#include "C8051f93x_SysClk.h"

/******************************************************************************
** 函 数 名: void CloseWT(void)
** 功能描述: 系统配置初始化: 关闭看门狗
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.04
******************************************************************************/ 
void CloseWT(void)
{
   PCA0MD &= ~0x40;                    		// WDTE = 0 禁止看门狗定时器
}

/******************************************************************************
** 函 数 名: SysClkInit
** 功能描述: 系统时钟初始化
**           
** 输　  入: uint8 ClkType, uint8 ClkDIV, uint8 Xoscmd, uint8 Freq
**			ClkType:  	CLK_INT_OSC		使用精准内部振荡器时钟
**                     	CLK_EXT_OSC		使用外部晶体振荡器时钟
**					   	CLK_smaRTClock_OSC	使用smaRTClock振荡器时钟
**					   	CLK_LOW_POWER_OSC	使用低功耗内部振荡器时钟
**			ClkDIV:(分频系数)
**						OSC_SYSCLK_Div1
**						OSC_SYSCLK_Div2
**						OSC_SYSCLK_Div4
**						OSC_SYSCLK_Div8
**						OSC_SYSCLK_Div16
**						OSC_SYSCLK_Div32
**						OSC_SYSCLK_Div64
**						OSC_SYSCLK_Div128
**			Xoscmd: (外部振荡器方式)
**                     	EXT_OSC_DISABLE		外部振荡器电路禁止
** 						EXT_OSC_CMOS	 	外部CMOS时钟方式
** 						EXT_OSC_CMOS_Div2 	外部CMOS时钟方式二分频
** 						EXT_OSC_RC		 	外部RC振荡器方式
**						EXT_OSC_C		 	外部电容振荡器方式
** 						EXT_OSC_CRY	 		外部晶体振荡器方式
** 						EXT_OSC_CRY_Div2	外部晶体振荡器方式二分频
**			Freq：（外部振荡器频率控制XFCN）(如果选择外部晶振)	   	
**                 		 							   晶体频率					（RC和 C方式） 
** 						EXT_OSC_FREQUENCY_RANGE0		f ≤ 20kHz 			    f ≤ 25 kHz 
** 						EXT_OSC_FREQUENCY_RANGE1	20 kHz < f ≤ 58 kHz			25 kHz < f ≤ 50 kHz 
** 						EXT_OSC_FREQUENCY_RANGE2 	58 kHz < f ≤ 155 kHz		50 kHz < f ≤ 100 kHz 
** 						EXT_OSC_FREQUENCY_RANGE3	155 kHz < f ≤ 415 kHz		100 kHz < f ≤ 200 kHz 
** 						EXT_OSC_FREQUENCY_RANGE4	415 kHz < f ≤ 1.1 MHz		200 kHz < f ≤ 400 kHz 
**						EXT_OSC_FREQUENCY_RANGE5	1.1 MHz < f ≤ 3.1 MHz		400 kHz < f ≤ 800 kHz 
**						EXT_OSC_FREQUENCY_RANGE6	3.1 MHz < f ≤ 8.2 MHz		800 kHz < f ≤ 1.6 MHz 
**						EXT_OSC_FREQUENCY_RANGE7	8.2 MHz < f ≤ 25 MHz		1.6 MHz < f ≤ 3.2 MHz
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.04
******************************************************************************/ 
void SysClkInit(uint8 ClkType, uint8 ClkDIV, uint8 Xoscmd, uint8 Freq)	
{
	//uint16 i = 0;

   //uint8 SFRPAGE_SAVE = SFRPAGE;    	// 保存当前SFRPAGE
   //SFRPAGE = CONFIG_PAGE;              	// 改变页

   switch(ClkType)
   {
		case CLK_INT_OSC:				//使用内部振荡器时钟(上电复位后就使用这种方式)
			//改变时钟频率时,先切换到内部低功耗振荡器,保证系统正常运行
			//CLKSEL = 0x40;							
			OSCICN = OSCICN | 0x80;			
			while((OSCICN & 0x40) == 0);//等待内部振荡器频率准备好标志
			CLKSEL = 0x00 | ClkDIV;  	//将系统时钟源切换到内部振荡器
			while((CLKSEL & 0x80) == 0);//等待时钟分频设置应用于系统时钟
			break;
		case CLK_EXT_OSC:			//使用外部晶体振荡器时钟
			//改变时钟频率时,先切换到内部低功耗振荡器,保证系统正常运行
			//CLKSEL = 0x40;
			OSCICN = OSCICN | 0x80;			
			//设置外部时钟
			OSCXCN |= (Xoscmd | Freq);		// 晶体振荡器方式,外部振荡器偏置电流
			if((Xoscmd != EXT_OSC_CMOS )&&(Xoscmd != EXT_OSC_CMOS_Div2) )
			{
				while(!(OSCXCN & 0x80));		// 等待晶体振荡器稳定运行
			}
			CLKSEL = 0x01 | ClkDIV;  	  // 选择外部振荡器，系统时钟分频 
			while((CLKSEL & 0x80) == 0);//等待时钟分频设置应用于系统时钟
			OSCICN &= ~0x80;			  // 禁止内部振荡器
			break;
		case CLK_smaRTClock_OSC:	//使用smaRTClock振荡器时钟			
			break;
		case CLK_LOW_POWER_OSC:	    //使用低功耗内部振荡器时钟
			//CLKSEL = 0x40;
			break;
		default:		//使用内部振荡器时钟(上电复位后就使用这种方式)
			//CLKSEL = 0x40;
			OSCICN = OSCICN | 0x80;
			break;
   }
   	RSTSRC = 0x06;                      			// 使能时钟丢失检测器（MCD）
                                       				// 使能VDD/DC+电源监视器为复位源
}


