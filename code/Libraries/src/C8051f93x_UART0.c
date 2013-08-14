/**************************************************************************************************
** �� �� ����C8051f93x_UART0.c
** ��    ��: xzy    �汾: v01     ��д����: 2013.04    �������: 2013
** ��������:1. ʹ�ö�ʱ��1��Ϊ�����ʷ����� 
**------------------------------------------------------------------------------------
** ��    ��: 1. ��������Ϊ:
**              P0.4-----TXD
**              P0.5-----RXD
**              VCC------+3V
**              GND------GND         
**---------------------------------------------------------------------------------------------------
** Ӳ��ƽ̨: 
**           ML-EC5������;
**           
**																					 
** ���ƽ̨: KEIL4.10
**---------------------------------------------------------------------------------------------------
** �޸ļ�¼: 
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
** �� �� ��: UART0Init
** ��������: UART0��ʼ��
** �䡡  ��: uint8 UART0Mode
**           UART0Mode: UART0ģʽ UART0_MODE0��UART0_MODE1�� UART0_MODE2�� UART0_MODE3(��C8051f93x_UART0.h)
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04     	
******************************************************************************/ 
void UART0Init(uint8 UART0Mode)
{
	uint8 	i;
	//Bit7: S0MODE�����пڹ�����ʽ
	//Bit5�� SM20���ദ����ͨ��ʹ��λ
	//Bit4�� REN0����������
	//Bit3�� TB80���ھŷ���λ
	//Bit2�� RB80���ھŽ���λ
	//Bit1�� TI0�������жϱ�־
	//Bit0�� RI0�������жϱ�־
   	SCON0 = 0x10 | UART0Mode << 7;
	/*	����23.1A ʹ�ö�ʱ��1 �ķ�ʽ2 ������
	    ������ = 1/2 �� ��ʱ��1 �����	=> T1OV = BAUD * 2			
		����23.1B ��ʱ�������
		��ʱ��1 ����� = T1CLK /��256 �D TH1��	 => TH1= 256-T1CLK/T1OV		
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
   Timer1Open();                         // ������ʱ��1
   UART0IntPriorH();                    // UART ���ж����ȼ�
#if	UART0_INTERRUPT_EN > 0
   //UART0IntSwitchOn();                  // ʹ��UART0�ж�
#endif
	
}
/******************************************************************************
** �� �� ��: UUART0SendByte
** ��������: ͨ��UART0����һ���ֽ�
** �䡡  ��: Val �����͵��ֽ�
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04.27    	
******************************************************************************/
/*void UART0SendByte(uint8 Val)
{
	if(ES0)									//��������жϿ�
	{
	#if	UART0_INTERRUPT_EN > 0
		UART0IntSwitchOff();				//UART0�жϹ�
	#endif
		SCON0 &= ~0x02;						//���TI
		SBUF0 = Val;
		while((SCON0 & 0x02) == 0);			//�ȴ�������� 
		SCON0 &= ~0x02;						//���TI
	#if	UART0_INTERRUPT_EN > 0
		UART0IntSwitchOn();					//UART0�жϿ�
	#endif	
	}
	else
	{
		SCON0 &= ~0x02;						//���TI
		SBUF0 = Val;
		while((SCON0 & 0x02) == 0);			//�ȴ�������� 
		SCON0 &= ~0x02;						//���TI
	}
} */


/******************************************************************************
** �� �� ��: UART0_ReadyRecv
** ��������: UART0׼���������ֽ�
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04.27   	
******************************************************************************/
/*void UART0_ReadyRecv(void)
{
	SCON0 &= ~0x01;							//���RI 
} */

/******************************************************************************
** �� �� ��: UART0_RecvNewDat
** ��������: �Ƿ���������
** �䡡  ��: ��
** �䡡  ��: stat  Ϊ0: ��������
**                 Ϊ1: ��������
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04.27   	
******************************************************************************/
/*uint8 UART0_RecvNewDat(void)
{
	uint8 	stat;
	stat     = SCON0 & 0x01;				//�õ�RI 
	return   stat;
}*/

/******************************************************************************
** �� �� ��: UART0_RecvOneDat
** ��������: ����һ���ֽ�����
** �䡡  ��: ��
** �䡡  ��: Dat ���յ�һ���ֽ�����
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013.04.27    	
******************************************************************************/
/*uint8 UART0_RecvOneDat(void)
{
	uint8 	Dat; 
	Dat      = SBUF0;
	SCON0 	&= ~0x01;						//���RI
	return   Dat;
}*/
/*****************************************************************************/
/*******************************�жϷ������**********************************/
/*****************************************************************************/

/******************************************************************************
** �� �� ��: UART0Int
** ��������: UART0�жϺ���
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: UART0RecvBuf
** ����ģ��: 
** ����  ��: xzy
** �ա�  ��: 2013.04.27
******************************************************************************/ 
#if	UART0_INTERRUPT_EN > 0
void UART0Int(void) interrupt UART0_PRIOR_ORDER
{

	/*if(SCON0 & 0x01)						//�����жϱ�־��Ч
	{
		UART0RecvBuf = SBUF0;
		SCON0 &= ~0x01;						//���RI
		UART0RecvIntFlag = FLAG_SET;
	}
	if(SCON0 & 0x02)						//�����жϱ�־��Ч
	{
		SCON0 &= ~0x02;						//���TI
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
