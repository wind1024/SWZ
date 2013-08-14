/*
*************************************************************************************************
** �� �� ����Interrupt.h
** ��    ��: xzy    �汾: v01     ��д����: 2013    �������: 2013
** ��������: �ж����������жϿ���                       
**---------------------------------------------------------------------------------------------------
** Ӳ��ƽ̨: 
**           ML-EC5������;        																					 
** ���ƽ̨: KEIL4.10
**---------------------------------------------------------------------------------------------------
** �޸ļ�¼: 
** 			 
****************************************************************************************************
*/
#ifndef 	_INTERRUPT_H
#define 	_INTERRUPT_H

#include <compiler_defs.h>             		// compiler declarations
#include <C8051F930_defs.h> 				// SFR declarations
//#include "Compile.h"	

//���ȼ�˳���壨��C8051F9xx ���İ�.pdf��P111��
//��λ�жϣ��ж�������ַ0x0000; �жϱ�־λ (��); �ж�ʹ��λ(һֱʹ��); �ж����ȼ�����λ(�������) 
//#define	RST_PRIOR_ORDER			//��� 

//�ⲿ�ж�0���ж�������ַ0x0003; �жϱ�־λIE0(TCON.1); �ж�ʹ��λEX0(IE.0); �ж����ȼ�����λPX0(IP.0) 
#define		EINT0_PRIOR_ORDER		0  	

//��ʱ���ж�0���ж�������ַ0x000B; �жϱ�־λTF0(TCON.5); �ж�ʹ��λET0(IE.1); �ж����ȼ�����λPT0(IP.1)
#define		TIMER0_PRIOR_ORDER		1

//�ⲿ�ж�1���ж�������ַ0x0013; �жϱ�־λIE1 (TCON.3); �ж�ʹ��λEX1 (IE.2); �ж����ȼ�����λPX1 (IP.2) 
#define		EINT1_PRIOR_ORDER		2  	

//��ʱ���ж�1���ж�������ַ0x001B; �жϱ�־λTF1 (TCON.7); �ж�ʹ��λET1 (IE.3); �ж����ȼ�����λPT1 (IP.3)
#define		TIMER1_PRIOR_ORDER		3

//���п��ж�0���ж�������ַ0x0023; �жϱ�־λRI0(SCON0.0), TI0(SCON0.1); �ж�ʹ��λES0(IE.4); �ж����ȼ�����λPS0(IP.4)
#define		UART0_PRIOR_ORDER		4  	

//��ʱ���ж�2���ж�������ַ0x002B; �жϱ�־λTF2(TMR2CN.7), EXF2(TMR2CN.6); �ж�ʹ��λET2(IE.5); �ж����ȼ�����λPT2(IP.5)
#define		TIMER2_PRIOR_ORDER		5

//SPI0�жϣ��ж�������ַ0x0033; �жϱ�־λSPIF(SPI0CN.7), WCOL(SPI0CN.6), MODF(SPI0CN.5), RXOVRN(SPI0CN.4); 
//         �ж�ʹ��λESPI0(IE.6); �ж����ȼ�����λPSPI0(IP.6) 
#define		SPI0_PRIOR_ORDER		6  	

//SMBus�жϣ��ж�������ַ0x003B; �жϱ�־λSI(SMB0CN.0); �ж�ʹ��λESMB0(EIE1.0); �ж����ȼ�����λPSMB0(EIP1.0)
#define		SMBUS_PRIOR_ORDER		7

//smaRTClock�澯�жϣ��ж�������ַ0x0043; �жϱ�־λALRM(RTC0CN.2); �ж�ʹ��λERTC0A(EIE1.1); �ж����ȼ�����λPRTC0A(EIP1.1)
#define		RTC0_ALARM_PRIOR_ORDER	8   

//ADC0���ڱȽ��жϣ��ж�������ַ0x004B; �жϱ�־λAD0WINT(ADC0CN.3); �ж�ʹ��λEWADC0(EIE1.2); �ж����ȼ�����λPWADC0(EIP1.2) 
#define		ADC0_WINDOW_CMP_PRIOR_ORDER		9  	

//ADC0ת�������жϣ��ж�������ַ0x0053; �жϱ�־λAD0INT(ADC0CN.5); �ж�ʹ��λEADC0(EIE1.3); �ж����ȼ�����λPADC0(EIP1.3) 
#define		ADC0_END_CONVERT_PRIOR_ORDER	10  	

//PCA0�жϣ��ж�������ַ0x005B; �жϱ�־λCF(PCA0CN.7), CCFn(PCA0CN.n); �ж�ʹ��λEPCA0(EIE1.4); �ж����ȼ�����λPPCA0(EIP1.4)
#define		PCA0_PRIOR_ORDER		11

//�Ƚ���0�жϣ��ж�������ַ0x0063; �жϱ�־λCP0FIF(CPT0CN.4),CP0FIF(CPT0CN.5); �ж�ʹ��λECP0(EIE1.5); �ж����ȼ�����λPCP0(EIP1.5) 
#define		COMPARATOR0_PRIOR_ORDER		12  		

//�Ƚ���1�жϣ��ж�������ַ0x006B; �жϱ�־λCP1FIF(CPT1CN.4),CP1FIF(CPT1CN.5); �ж�ʹ��λECP1(EIE1.6); �ж����ȼ�����λPCP1(EIP1.6) 
#define		COMPARATOR1_PRIOR_ORDER		13  	

//��ʱ��3�жϣ��ж�������ַ0x0073; �жϱ�־λTF3H(TMR3CN.7),TF3L(TMR3CN.6); �ж�ʹ��λET3(EIE1.7); �ж����ȼ�����λPT3(EIP1.7) 
#define		TIMER3_PRIOR_ORDER			14  	

//VDD/DC+��Դ���������ڸ澯�жϣ��ж�������ַ0x007B; �жϱ�־λVDDOK(VDM0CN.5); �ж�ʹ��λEWARN(EIE2.0); �ж����ȼ�����λPWARN(EIP2.0) 
#define		VDD_WARNING_PRIOR_ORDER		15  	

//�˿�ƥ���жϣ��ж�������ַ0x0083; �жϱ�־λ ��; �ж�ʹ��λEMAT(EIE2.1); �ж����ȼ�����λPMAT(EIP2.1)
#define		PORT_MATCH_PRIOR_ORDER		16

//smaRTClock ���������жϣ��ж�������ַ0x008B; �жϱ�־λOSCFAIL(RTC0CN.5); �ж�ʹ��λERTC0F(EIE2.2); �ж����ȼ�����λPRTC0F(EIP2.2)
#define		RTC0_OSC_FAIL_PRIOR_ORDER	17

//SPI1�жϣ��ж�������ַ0x0093; �жϱ�־λSPIF(SPI1CN.7), WCOL(SPI1CN.6), MODF(SPI1CN.5), RXOVRN(SPI1CN.4); 
//         �ж�ʹ��λESPI1(EIE2.3); �ж����ȼ�����λPSPI1(EIP2.3) 
#define		SPI1_PRIOR_ORDER		18  	



/* �궨�庯������ */
//�ж�����Ĵ���IE��������ҳ����
//��չ�ж�����Ĵ���1EIE1��������ҳ����
//��չ�ж�����Ĵ���1EIE2��������ҳ����
//�����ж�
/* IE�Ĵ��� */
#define	GlobleIntSwitchOn()		EA   = 1 			//ȫ���жϿ��� ��
#define	SPI0IntSwitchOn()		ESPI0 = 1			//����������ӿ�0��SPI0���ж�
#define	Timer2IntSwitchOn()		ET2  = 1			//��ʱ��2�жϿ�
#define	UART0IntSwitchOn()		ES0  = 1			//UART0�жϿ�	
#define	Timer1IntSwitchOn()		ET1  = 1 			//��ʱ��1�жϿ�
#define	EINT1IntSwitchOn()		EX1  = 1			//�ⲿ�ж�1�жϿ�
#define	Timer0IntSwitchOn()		ET0  = 1			//��ʱ��0�жϿ�	
#define	EINT0IntSwitchOn()		EX0  = 1			//�ⲿ�ж�0�жϿ�

/* EIE1 �Ĵ��� */
#define Timer3IntSwitchOn()		EIE1 |= (0x01 << 7) //��ʱ��3�жϿ�	
#define	CP1IntSwitchOn()		EIE1 |= (0x01 << 6) //����Ƚ���1��CP1���ж�
#define	CP0IntSwitchOn()		EIE1 |= (0x01 << 5) //����Ƚ���0��CP0���ж�	
#define	PCA0IntSwitchOn()		EIE1 |= (0x01 << 4) //����ɱ�̼��������У�PCA0���ж�
#define	ADC0IntSwitchOn()		EIE1 |= (0x01 << 3) //����ADC0 ת�������ж�
#define	WADC0IntSwitchOn()		EIE1 |= (0x01 << 2) //����ADC0 ���ڱȽ��ж�
#define	RTC0AIntSwitchOn()		EIE1 |= (0x01 << 1)	//����smaRTClock�澯�ж�	
#define	SMB0IntSwitchOn()		EIE1 |= 0x01		//����SMBus0 �ж�


/* EIE2 �Ĵ��� */
#define	SPI1IntSwitchOn()		EIE2 |= (0x01 << 3) //����������ӿ�1��SPI1���ж�
#define	RTC0FIntSwitchOn()		EIE2 |= (0x01 << 2) //����smaRTClock ���������ж�
#define	MATIntSwitchOn()		EIE2 |= (0x01 << 1) //����˿�ƥ���ж�
#define	WARNIntSwitchOn()		EIE2 |= 0x01 		//����VDD/DC+��Դ���������ڸ澯�ж�
		
//�ر��ж�
#define	GlobleIntSwitchOff()	EA   = 0 			//ȫ���жϿ��� �ر�
#define	SPI0IntSwitchOff()		ESPI0  = 0			//��������ӿ�0��SPI0���жϹر�
#define	Timer2IntSwitchOff()	ET2  = 0			//��ʱ��2�жϹر�
#define	UART0IntSwitchOff()		ES0  = 0			//UART0�жϹر�	
#define	Timer1IntSwitchOff()	ET1  = 0 			//��ʱ��1�жϹر�
#define	EINT1IntSwitchOff()		EX1  = 0			//�ⲿ�ж�1�жϹر�
#define	Timer0IntSwitchOff()	ET0  = 0			//��ʱ��0�жϹر�	
#define	EINT0IntSwitchOff()		EX0  = 0			//�ⲿ�ж�0�жϹر�

#define	Timer3IntSwitchOff()	EIE1 &= (~(0x01 << 7)) //��ʱ��3�жϹر�
#define	CP1FIntSwitchOff()		EIE1 &= (~(0x01 << 6)) //�Ƚ���1��CP1���жϹر�
#define	CP0RIntSwitchOff()		EIE1 &= (~(0x01 << 5)) //�Ƚ���0��CP0���жϹر�	
#define	PCA0IntSwitchOff()		EIE1 &= (~(0x01 << 4)) //�ɱ�̼��������У�PCA0���жϹر�
#define	ADC0IntSwitchOff()		EIE1 &= (~(0x01 << 3)) //ADC0 ת�������жϹر�
#define	WADC0IntSwitchOff()		EIE1 &= (~(0x01 << 2)) //ADC0 ���ڱȽ��жϹر�	
#define	RTC0AIntSwitchOFF()		EIE1 &= (~(0x01 << 1)) //smaRTClock�澯�жϹر�
#define	SMB0IntSwitchOff()		EIE1 &= ~0x01		 //SMBus0 �жϹر�

#define	SPI1IntSwitchOff()		EIE2 &= (~(0x01 << 3)) //��������ӿ�1��SPI1���жϹر�	
#define	RTC0FIntSwitchOff()		EIE2 &= (~(0x01 << 2)) //smaRTClock ���������жϹر�
#define	MATIntSwitchOff()		EIE2 &= (~(0x01 << 1)) //�˿�ƥ���жϹر�
#define	WARNIntSwitchOff()		EIE2 &= ~0x01 		 //VDD/DC+��Դ���������ڸ澯�жϹر�


//�ж����ȼ��Ĵ���IP��������ҳ����
//��չ�ж����ȼ��Ĵ���EIP1��������ҳ����
//��չ�ж����ȼ��Ĵ���EIP2��������ҳ����
/* IP�Ĵ��� */
#define SPI0IntPriorH()			PSPI0 = 1			//��������ӿڣ�SPI0���ж����ȼ���
#define	Timer2IntPriorH()		PT2  = 1			//��ʱ��2�ж����ȼ���
#define	UART0IntPriorH()		PS0  = 1			//UART0�ж����ȼ���	
#define	Timer1IntPriorH()		PT1  = 1 			//��ʱ��1�ж����ȼ���
#define	EINT1IntPriorH()		PX1  = 1			//�ⲿ�ж�1�ж����ȼ���
#define	Timer0IntPriorH()		PT0  = 1			//��ʱ��0�ж����ȼ���	
#define	EINT0IntPriorH()		PX0  = 1			//�ⲿ�ж�0�ж����ȼ���

/* EIP1 �Ĵ��� */
#define	Timer3IntPriorH()		EIP1 |= (0x01 << 7) //����ʱ��3 �ж����ȼ���
#define	CP1IntPriorH()			EIP1 |= (0x01 << 6) //����Ƚ���1��CP1���ж����ȼ���
#define	CP0IntPriorH()			EIP1 |= (0x01 << 5) //����Ƚ���0��CP0���ж����ȼ���	
#define	PCA0IntPriorH()			EIP1 |= (0x01 << 4) //����ɱ�̼��������У�PCA0���ж����ȼ���
#define	ADC2IntPriorH()			EIP1 |= (0x01 << 3) //����ADC2 ת�������ж����ȼ���
#define	WADC0IntPriorH()		EIP1 |= (0x01 << 2) //����ADC0 ���ڱȽ��ж����ȼ���	
#define	RTC0AIntPriorH()		EIP1 |= (0x01 << 1) //����smaRTClock�澯�ж����ȼ���
#define	SMB0IntPriorH()			EIP1 |= 0x01		//����SMBus0 �ж����ȼ���

/* EIP2 �Ĵ��� */
#define	SPI1IntPriorH()			EIP2 |= (0x01 << 3) //����������ӿڣ�SPI1���ж����ȼ���
#define	RTC0FIntPriorH()		EIP2 |= (0x01 << 2) //����smaRTClock ���������ж����ȼ���
#define	MATIntPriorH()			EIP2 |= (0x01 << 1) //����˿�ƥ���ж����ȼ���
#define	WARNIntPriorH()			EIP2 |= 0x01 		 //����VDD/DC+��Դ���������ڸ澯�ж����ȼ���

//����Ϊ�����ȼ�
/* IP�Ĵ��� */
#define SPI0IntPriorL()			PSPI0 = 0			//��������ӿڣ�SPI0���ж����ȼ���
#define	Timer2IntPriorL()		PT2  = 0			//��ʱ��2�ж����ȼ���
#define	UART0IntPriorL()		PS0  = 0			//UART0�ж����ȼ���
#define	Timer1IntPriorL()		PT1  = 0 			//��ʱ��1�ж����ȼ���
#define	EINT1IntPriorL()		PX1  = 0			//�ⲿ�ж�1�ж����ȼ���
#define	Timer0IntPriorL()		PT0  = 0			//��ʱ��0�ж����ȼ���	
#define	EINT0IntPriorL()		PX0  = 0			//�ⲿ�ж�0�ж����ȼ���

/* EIP1 �Ĵ��� */
#define	Timer3IntPriorL()		EIP1 &= (~(0x01 << 7)) //����ʱ��3 �ж����ȼ���
#define	CP1IntPriorL()			EIP1 &= (~(0x01 << 6)) //����Ƚ���1��CP1���ж����ȼ���
#define	CP0IntPriorL()			EIP1 &= (~(0x01 << 5)) //����Ƚ���0��CP0���ж����ȼ���	
#define	PCA0IntPriorL()			EIP1 &= (~(0x01 << 4)) //����ɱ�̼��������У�PCA0���ж����ȼ���
#define	ADC2IntPriorL()			EIP1 &= (~(0x01 << 3)) //����ADC2 ת�������ж����ȼ���
#define	WADC0IntPriorL()		EIP1 &= (~(0x01 << 2)) //����ADC0 ���ڱȽ��ж����ȼ���	
#define	RTC0AIntPriorL()		EIP1 &= (~(0x01 << 1)) //����smaRTClock�澯�ж����ȼ���
#define	SMB0IntPriorL()			EIP1 &= ~0x01		//����SMBus0 �ж����ȼ���

/* EIP2 �Ĵ��� */
#define	SPI1IntPriorL()			EIP2 &= (~(0x01 << 3)) //����������ӿڣ�SPI1���ж����ȼ���
#define	RTC0FIntPriorL()		EIP2 &= (~(0x01 << 2)) //����smaRTClock ���������ж����ȼ���
#define	MATIntPriorL()			EIP2 &= (~(0x01 << 1)) //����˿�ƥ���ж����ȼ���
#define	WARNIntPriorL()			EIP2 &= ~0x01 		 //����VDD/DC+��Դ���������ڸ澯�ж����ȼ���
// TCON ��ʱ�����ƼĴ��� 
#define EINT0LevelTrigger()		TCON &= ~0x01		//�ⲿ�ж�0��ƽ����
#define EINT1LevelTrigger()		TCON &= (~(0x01 << 2))//�ⲿ�ж�1��ƽ����

#define EINT0EdgeTrigger()		TCON |= 0x01		//�ⲿ�ж�0���ش���
#define EINT1EdgelTrigger()		TCON |= (0x01 << 2)	//�ⲿ�ж�1���ش���

//TMRnCN.5��ʱ�����ֽ��жϿ���
#define TIMER2LENIntON()		TF2LEN = 1			//��ʱ��2 ���ֽ��ж�����
#define TIMER3LENIntON()		TMR3CN |= (0x01 << 5)	//��ʱ��3 ���ֽ��ж�����

#define TIMER2LENIntOFF()		TF2LEN = 0			//��ʱ��2 ���ֽ��жϽ�ֹ
#define TIMER3LENIntOFF()		TMR3CN &= (~(0x01 << 5))	//��ʱ��3 ���ֽ��жϽ�ֹ


#endif