/**************************************************************************************************
** 文 件 名：C8051f93x_UART0.c
** 作    者: xzy    版本: v01     编写日期: 2013.04    完成日期: 2013
** 功能描述:1. 使用定时器1作为波特率发生器 
**------------------------------------------------------------------------------------
** 其    他: 1. 引脚设置为:
**              P0.4-----TXD
**              P0.5-----RXD
**              VCC------+3V
**              GND------GND         
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
#include "pin_define.h"
#include "C8051f93x_UART0.h"
#include "Interrupt.h"
#include "C8051f93x_Timer01.h"
#include "memory.h"

#if	UART0_INTERRUPT_EN > 0
//uint8 	UART0RecvBuf;
//uint8 	UART0RecvIntFlag;
#endif

/******************************************************************************
** 函 数 名: UART0Init
** 功能描述: UART0初始化
** 输　  入: uint8 UART0Mode
**           UART0Mode: UART0模式 UART0_MODE0，UART0_MODE1， UART0_MODE2， UART0_MODE3(见C8051f93x_UART0.h)
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04     	
******************************************************************************/ 
void UART0Init(uint8 UART0Mode)
{
	uint8 	i;
	//Bit7: S0MODE：串行口工作方式
	//Bit5： SM20：多处理器通信使能位
	//Bit4： REN0：接收允许
	//Bit3： TB80：第九发送位
	//Bit2： RB80：第九接收位
	//Bit1： TI0：发送中断标志
	//Bit0： RI0：接收中断标志
   	SCON0 = 0x10 | UART0Mode << 7;
	/*	方程23.1A 使用定时器1 的方式2 波特率
	    波特率 = 1/2 × 定时器1 溢出率	=> T1OV = BAUD * 2			
		方程23.1B 定时器溢出率
		定时器1 溢出率 = T1CLK /（256 ― TH1）	 => TH1= 256-T1CLK/T1OV		
		==>> TH1 = 256-T1CLK/(BAUD * 2) = 256 - T1CLK /BAUD / 2
	*/
	#if (SYSCLK/BAUDRATE/2/256 < 1) 
      i = -(SYSCLK/BAUDRATE/2);
	  Timer01Init(1,TIMER01_MODE2_8BIT_RELOAD,TIMER01_CLKSOURCE_SYSCLK);
      //CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      //CKCON |=  0x08;
   #elif (SYSCLK/BAUDRATE/2/256 < 4) 
      i = -(SYSCLK/BAUDRATE/2/4);
	  Timer01Init(1,TIMER01_MODE2_8BIT_RELOAD,TIMER01_CLKSOURCE_SYSCLK_4);
     // CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
     // CKCON |=  0x01;
   #elif (SYSCLK/BAUDRATE/2/256 < 12) 
      i = -(SYSCLK/BAUDRATE/2/12);
	  Timer01Init(1,TIMER01_MODE2_8BIT_RELOAD,TIMER01_CLKSOURCE_SYSCLK_12);
     // CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   #else 
      i = -(SYSCLK/BAUDRATE/2/48);
	  Timer01Init(1,TIMER01_MODE2_8BIT_RELOAD,TIMER01_CLKSOURCE_SYSCLK_48);
      //CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      //CKCON |=  0x02;
   #endif
   Timer1SetValue(i*256 + i);			 // TL1 = TH1; 
   Timer1Open();                         // 开启定时器1
   UART0IntPriorH();                    // UART 高中断优先级
#if	UART0_INTERRUPT_EN > 0
   //UART0IntSwitchOn();                  // 使能UART0中断
#endif
	
}
/******************************************************************************
** 函 数 名: UUART0SendByte
** 功能描述: 通过UART0发送一个字节
** 输　  入: Val 待发送的字节
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04.27    	
******************************************************************************/
/*void UART0SendByte(uint8 Val)
{
	if(ES0)									//如果串口中断开
	{
	#if	UART0_INTERRUPT_EN > 0
		UART0IntSwitchOff();				//UART0中断关
	#endif
		SCON0 &= ~0x02;						//清除TI
		SBUF0 = Val;
		while((SCON0 & 0x02) == 0);			//等待发送完成 
		SCON0 &= ~0x02;						//清除TI
	#if	UART0_INTERRUPT_EN > 0
		UART0IntSwitchOn();					//UART0中断开
	#endif	
	}
	else
	{
		SCON0 &= ~0x02;						//清除TI
		SBUF0 = Val;
		while((SCON0 & 0x02) == 0);			//等待发送完成 
		SCON0 &= ~0x02;						//清除TI
	}
} */


/******************************************************************************
** 函 数 名: UART0_ReadyRecv
** 功能描述: UART0准备接收新字节
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04.27   	
******************************************************************************/
/*void UART0_ReadyRecv(void)
{
	SCON0 &= ~0x01;							//清除RI 
} */

/******************************************************************************
** 函 数 名: UART0_RecvNewDat
** 功能描述: 是否有新数据
** 输　  入: 无
** 输　  出: stat  为0: 无新数据
**                 为1: 有新数据
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04.27   	
******************************************************************************/
/*uint8 UART0_RecvNewDat(void)
{
	uint8 	stat;
	stat     = SCON0 & 0x01;				//得到RI 
	return   stat;
}*/

/******************************************************************************
** 函 数 名: UART0_RecvOneDat
** 功能描述: 接收一个字节数据
** 输　  入: 无
** 输　  出: Dat 接收的一个字节数据
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 编写日期: 2013.04.27    	
******************************************************************************/
/*uint8 UART0_RecvOneDat(void)
{
	uint8 	Dat; 
	Dat      = SBUF0;
	SCON0 	&= ~0x01;						//清除RI
	return   Dat;
}*/
/*****************************************************************************/
/*******************************中断服务程序**********************************/
/*****************************************************************************/

/******************************************************************************
** 函 数 名: UART0Int
** 功能描述: UART0中断函数
** 输　  入: 无
** 输　  出: 无
** 全局变量: UART0RecvBuf
** 调用模块: 
** 作　  者: xzy
** 日　  期: 2013.04.27
******************************************************************************/ 
#if	UART0_INTERRUPT_EN > 0
void UART0Int(void) interrupt UART0_PRIOR_ORDER
{

	/*if(SCON0 & 0x01)						//接收中断标志有效
	{
		UART0RecvBuf = SBUF0;
		SCON0 &= ~0x01;						//清除RI
		UART0RecvIntFlag = FLAG_SET;
	}
	if(SCON0 & 0x02)						//发送中断标志有效
	{
		SCON0 &= ~0x02;						//清除TI
	} */
	GlobleIntSwitchOff();
	if(RI0 && PC_SET)	
	{	 
		RI0=0;
		if(LinkDat->WAVE_Receive_Num>99)						// added 3.15.13
		{
			RECEIVE_FLAG = 0;
			LinkDat->WAVE_Receive_Num = 0;
		}
		if(RECEIVE_FLAG==0)
		{
			if(SBUF0==0x7E)
			{
				RECEIVE_FLAG=1;
				LinkDat->RECEIVEBUFFERPOINT = LinkDat->RECEIVEBUFFER;
				LinkDat->WAVE_Receive_Num = 0;
			}
        }
		else
		{

			if(SBUF0==0x7E)
			{ 
				RECEIVE_FLAG=0;	
			  	if(LinkDat->RECEIVEBUFFERPOINT != LinkDat->RECEIVEBUFFER) 		// Avoid the 0x7e and 0x7e but length = 0
				{	
					PPP_Rec_Complete = 1;
                }
			}
			else
			{
				if(PADDING_Flag==1)
	  			{
	  				*LinkDat->RECEIVEBUFFERPOINT=SBUF0;
					if((*LinkDat->RECEIVEBUFFERPOINT) == 0x5E)
					{
						*LinkDat->RECEIVEBUFFERPOINT = 0x7E;
					}
					else if((*LinkDat->RECEIVEBUFFERPOINT) == 0x5D)
					{
						*LinkDat->RECEIVEBUFFERPOINT = 0x7D;
					}
					else
					{
						*LinkDat->RECEIVEBUFFERPOINT^=0x20;
					}	  	        	
	  	        	PADDING_Flag=0;
	  	        	LinkDat->RECEIVEBUFFERPOINT++;
					LinkDat->WAVE_Receive_Num++;
	    		}
	    		else
	    		{
	    			if(SBUF0==0x7D)
	    				PADDING_Flag=1;
	    			else
	    			{
	    				*LinkDat->RECEIVEBUFFERPOINT=SBUF0;
	  	           		LinkDat->RECEIVEBUFFERPOINT++;
						LinkDat->WAVE_Receive_Num++;
	  	        	}
	  	        }
			}
		}
   	}
	else if(RI0 && !PC_SET)
	{
	}
    GlobleIntSwitchOn();
}
#endif
