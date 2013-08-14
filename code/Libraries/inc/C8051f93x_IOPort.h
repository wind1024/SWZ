/**************************************************************************************************
** �� �� ����C8051f93x_IOPort.h
** ��    ��: xzy    �汾: v01     ��д����: 2013    �������: 2013
** ��������:          
**---------------------------------------------------------------------------------------------------
** Ӳ��ƽ̨: 																					 
** ���ƽ̨: 
**---------------------------------------------------------------------------------------------------
** �޸ļ�¼: 
** 			 
*****************************************************************************************************/
#ifndef 	C8051f93x_IOPORT_H
#define 	C8051f93x_IOPORT_H

#include "compiler_defs.h"             		// compiler declarations
#include "C8051F930_defs.h" 				// SFR declarations
#include "Compile.h"	

#define NULL    0
/* �˿ڲ���ʱ�����,���Ժ�Ӧ�ر�(�Ѹ����������) */

//�˿ڵ����źŶ���
#define		PIN_BIT0				0x01 			//PINx.0
#define		PIN_BIT1				0x02			//PINx.1
#define		PIN_BIT2				0x04			//PINx.2
#define		PIN_BIT3				0x08			//PINx.3
#define		PIN_BIT4				0x10			//PINx.4
#define		PIN_BIT5				0x20			//PINx.5
#define		PIN_BIT6				0x40			//PINx.6
#define		PIN_BIT7				0x80			//PINx.7

//���ŷ�ʽ����ѡ��

#define 	PUSH_PULL_OUT   	1					//�����������
#define		OPEN_DRAIN_OUT		2					//���������©

//ͨ�����������ʽΪ"©����·"����˿����ݼĴ����е���Ӧλд'1'���˿���������Ϊ�������� 
#define		DIGITAL_INPUT		3					//������������

//ֻ��P1�ڿ��Ա�����Ϊģ������(ADC2)����Ϊֻ��P1����P1MDIN�Ĵ���
//������Ϊģ����������ŵ������������û�б���ȷ�ؽ�ֹ��
//��˱�����Ϊģ���������������Ӧ��P1MDOUT λӦ����ȷ������Ϊ�߼�'0'��©����·��ʽ��,
//��Ӧ�Ķ˿�����λӦ������Ϊ�߼�'1'������̬��
//��ֹ������Ϊģ����������ŵ����������
#define		ANALOG_INPUT		4					//����ģ������(ֻ��P1�ڿ���)
//�ⲿ�ж����ŷ��䣨���ŷ����뽻�濪���޹أ�
#define INT0_PORT_PIN(pin)		IT01CF |= (pin)
#define INT1_PORT_PIN(pin)		IT01CF |= (pin << 4)
//���濪����������
#define IO_PORT_PIN_SKIP(port, pin)		P##port##SKIP |= (0x01 << (pin))

//mode = {PUSH_PULL_OUT, OPEN_DRAIN_OUT, DIGITAL_INPUT, ANALOG_INPUT}
//port = {0, ..., 7},    pin  = {0, ..., 7}
#define IO_PORT_PIN_MODE(port, pin, mode,outmode)  	\
do{                                  		\
	if (mode == DIGITAL_INPUT){            	\
		P##port##MDIN |= (0x01 << (pin)); \
		P##port        |= (0x01 << (pin));  \
		if (outmode == PUSH_PULL_OUT)     	\
	 		P##port##MDOUT |= (0x01 << (pin));  \
		else if(outmode == OPEN_DRAIN_OUT)      \
	 		P##port##MDOUT &= ~(0x01 << (pin)); \
	}										\
	else if(mode == ANALOG_INPUT) {			\
		P##port##MDIN  &= ~(0x01 << (pin)); \		
		P##port##MDOUT &= ~(0x01 << (pin));	\
		P##port##      |= (0x01 << (pin)); 	\
	}										\
}while(0)

//mode = {PUSH_PULL_OUT, OPEN_DRAIN_OUT, IGITAL_INPUT, ANALOG_INPUT}
//port = {0, ..., 7}
#define IO_PORT_MODE(port, mode,outmode)  	\
do{                                  		\
  	if(mode == DIGITAL_INPUT){				\
		P##port##MDIN = 0xff;				\
		P##port        |= 0xff;  			\
  		if (outmode == PUSH_PULL_OUT)          	\
     		P##port##MDOUT |= 0xff;  			\
  		else if(outmode == OPEN_DRAIN_OUT)     	\
     		P##port##MDOUT &= 0x00; 			\
	}										\
	else if(mode == ANALOG_INPUT) {			\
		P##port##MDIN  &= 0x00; 			\		
		P##port##MDOUT &= 0x00;				\
		P##port##      |= 0xff; 			\
	}										\
}while(0)



//��������Ľ��濪��ʹ��λ����(�����ȼ��ɸߵ�������)
/* 1. �����EMIFѡ���Ͷ˿�(�ļ�"EMIF.h"��: #define XRAM_PORT_HIGH_EN 0),
      ��ʹ�����߷Ǹ��÷�ʽ(�ļ�"EMIF.h"��: #define	XRAM_MULTIPLEX_EN 0),
      ��P1.6����/RD,P1.7����/WR,P1�������Ǹ��õ�ַ��,P2�������Ǹ��õ�ַ��,P3��������������

   2. �����EMIFѡ���Ͷ˿�(�ļ�"EMIF.h"��: #define XRAM_PORT_HIGH_EN 0),
      ��ʹ�����߸��÷�ʽ(�ļ�"EMIF.h"��: #define XRAM_MULTIPLEX_EN 1),
      ��P1.5����ALE,P1.6����/RD,P1.7����/WR,P2���������õ�ַ��,P3���������õ�ַ�� �� ��������
   
   3. ���P1�ڵ�ȫ����ĳ��λ����ADC2,��P1ȫ����ĳ��λ�����������濪��
*/
#define		PORT_UART0_EN			0x01			//XBR0.0(ʹ��2 ������TX0(P0.4),RX0(P0.5))
//��SPI ������Ϊ3 �߷�ʽʱ��NSS �źŲ�������˿�����
#define		PORT_SPI1_EN			0x40			//XBR1.6(ʹ��3 ����4 ������SCK,MISO,MOSI,NSS)��P1.0��ʼ
#define		PORT_SPI0_EN			0x02			//XBR0.1(ʹ��3 ����4 ������SCK,MISO,MOSI,NSS)��P0.0��ʼ
#define		PORT_SMB0_EN			0x04			//XBR0.2(ʹ��2 ������SDA,SCL)
#define		PORT_CP0_EN				0x10			//XBR0.4(ʹ��1 ������CP0)
#define		PORT_CP0A_EN			0x20			//XBR0.5(ʹ��1 ������CP0A)
#define		PORT_CP1_EN				0x40			//XBR0.6(ʹ��1 ������CP1)
#define		PORT_CP1A_EN			0x80			//XBR0.7(ʹ��1 ������CP1A)
#define		PORT_SYSCLK_EN			0x08			//XBR0.3(ʹ��1 ������SYSCLK)
//XBR1.[2:0](ʹ��6 ������CEX0,CEX1,CEX2,CEX3,CEX4,CEX5)
#define		PORT_PCA0M0_EN			0x01			//ʹ��1 ������CEX0			
#define		PORT_PCA0M0_1_EN		0x02			//ʹ��2 ������CEX0,CEX1
#define		PORT_PCA0M0_2_EN		0x03			//ʹ��3 ������CEX0,CEX1,CEX2
#define		PORT_PCA0M0_3_EN		0x04			//ʹ��4 ������CEX0,CEX1,CEX2,CEX3
#define		PORT_PCA0M0_4_EN		0x05			//ʹ��5 ������CEX0,CEX1,CEX2,CEX3,CEX4
#define		PORT_PCA0M0_5_EN		0x06			//ʹ��6 ������CEX0,CEX1,CEX2,CEX3,CEX4,CEX5
#define		PORT_ECI_EN				0x08			//XBR1.3(ʹ��1 ������ECI)
#define		PORT_T0_EN				0x10			//XBR1.4(ʹ��1 ������T0) 
#define		PORT_T1_EN				0x20			//XBR1.5(ʹ��1 ������T1)
	
//ʹ��2 ������TX0,RX0
#define		PortUART0En()	XBR0 |= PORT_UART0_EN		

//ʹ��3 ����4 ������SCK,MISO,MOSI,NSS
#define		PortSPI1En()	XBR1 |= PORT_SPI1_EN									
#define		PortSPI0En()	XBR0 |= PORT_SPI0_EN		

//ʹ��2 ������SDA,SCL		
#define		PortSMB0En()	XBR0 |= PORT_SMB0_EN

//ʹ��1 ������CP0	 	
#define		PortCP0En()		XBR0 |= PORT_CP0_EN		

//ʹ��1 ������CP0A	 	
#define		PortCP0AEn()	XBR0 |= PORT_CP0A_EN

//ʹ��1 ������CP1		
#define		PortCP1En()		XBR0 |= PORT_CP1_EN	

//ʹ��1 ������CP1A		
#define		PortCP1AEn()	XBR0 |= PORT_CP1A_EN

//ʹ��1 ������SYSCLK		
#define		PortSYSCLKEn()	XBR0 |= PORT_SYSCLK_EN

//ʹ��1 ������CEX0		
#define		PortPCA0M0En()	XBR1 = (XBR1 & (~0x07)) | PORT_PCA0M0_EN

//ʹ��2 ������CEX0,CEX1	
#define		PortPCA0M0_1En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_1_EN

//ʹ��3 ������CEX0,CEX1,CEX2
#define		PortPCA0M0_2En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_2_EN	

//ʹ��4 ������CEX0,CEX1,CEX2,CEX3	
#define		PortPCA0M0_3En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_3_EN

//ʹ��5 ������CEX0,CEX1,CEX2,CEX3,CEX4
#define		PortPCA0M0_4En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_4_EN

//ʹ��6 ������CEX0,CEX1,CEX2,CEX3,CEX4,CEX5
#define		PortPCA0M0_5En()	XBR1  = (XBR1 & (~0x07)) | PORT_PCA0M0_5_EN

//ʹ��1 ������ECI
#define		PortECI0En()	XBR1 |= PORT_ECI0_EN

//ʹ��1 ������T0		
#define		PortT0En()	XBR1 |= PORT_T0_EN

//ʹ��1 ������T1		
#define		PortT1En()	XBR1 |= PORT_T1_EN

//ʹ�����ŵ�ȫ������������
#define		GlobalWeakPullEn()				\			
do{                                  		\
  	XBR2   &= ~0x80;						\
}while(0)

//��ֹ���ŵ�ȫ������������
#define		GlobalWeakPullDisable()			\			
do{                                  		\
  	XBR2   |= 0x80;							\
}while(0)

//���濪��ʹ��
#define		CrossBarEn()					\			
do{                                  		\
  	XBR2    |= 0x40;						\
}while(0)

//���濪�ؽ���
#define		CrossBarDisable()				\			
do{                                  		\
  	XBR2    &= ~0x40;						\
}while(0)


/* ���庯������ */
extern void IOPortInit(void); 						//IO�ڳ�ʼ��


#endif
