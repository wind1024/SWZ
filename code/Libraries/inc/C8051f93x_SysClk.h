/***********************************************************************************
**�ļ�˵����C8051f93xʱ��ϵͳ����ͷ�ļ�                                                              
**�ļ����ƣ�C8051f93x_SysClk.h                                                      
**�������ڣ�2013.04                                                            
**�� �� �ˣ�XZY                                                                   
**---------------------------------------------------------------------------------*
**�޸����ڣ�                                                                       
**�޸�˵����                                                                       
***********************************************************************************/
#ifndef 	C8051F93X_SYSCLK_H
#define 	C8051F93X_SYSCLK_H

#include <compiler_defs.h>             		// compiler declarations
#include <C8051F930_defs.h> 				// SFR declarations
#include <Compile.h>

#define 	SYSCLK           18432000      	// SYSCLK Ƶ�� ��λHz
/* ����ģʽѡ�� */
#define		CLK_INT_OSC		0		//ʹ�þ�׼�ڲ�����ʱ��
#define		CLK_EXT_OSC		1		//ʹ���ⲿ��������ʱ��
#define		CLK_smaRTClock_OSC	3	    //ʹ��smaRTClock����ʱ��
#define		CLK_LOW_POWER_OSC	4		//ʹ�õ͹����ڲ�����ʱ��
								

/* ϵͳʱ�ӷ�Ƶѡ�� */
#define		OSC_SYSCLK_Div1		0x00
#define 	OSC_SYSCLK_Div2		0x10 
#define 	OSC_SYSCLK_Div4		0x20 
#define 	OSC_SYSCLK_Div8		0x30 
#define 	OSC_SYSCLK_Div16	0x40 
#define 	OSC_SYSCLK_Div32	0x50 
#define 	OSC_SYSCLK_Div64	0x60
#define 	OSC_SYSCLK_Div128	0x70   		

//�ⲿ������ʽ
#define		EXT_OSC_DISABLE	 	0x00 	//�ⲿ������·��ֹ
#define		EXT_OSC_CMOS	 	0x20 	//�ⲿCMOSʱ�ӷ�ʽ
#define		EXT_OSC_CMOS_Div2 	0x30 	//�ⲿCMOSʱ�ӷ�ʽ����Ƶ
#define		EXT_OSC_RC		 	0x40 	//�ⲿRC������ʽ
#define		EXT_OSC_C		 	0x50 	//�ⲿ����������ʽ
#define		EXT_OSC_CRY	 		0x60 	//�ⲿ����������ʽ
#define		EXT_OSC_CRY_Div2	0x70 	//�ⲿ����������ʽ����Ƶ
//�ⲿ����Ƶ�ʿ���XFCN 							   ����Ƶ��					��RC�� C��ʽ�� 
#define		EXT_OSC_FREQUENCY_RANGE0	0x00 	//	   f �� 20kHz 			    f �� 25 kHz 
#define		EXT_OSC_FREQUENCY_RANGE1	0x01 	//20 kHz < f �� 58 kHz		25 kHz < f �� 50 kHz 
#define		EXT_OSC_FREQUENCY_RANGE2 	0x02 	//58 kHz < f �� 155 kHz		50 kHz < f �� 100 kHz 
#define		EXT_OSC_FREQUENCY_RANGE3	0x03 	//155 kHz < f �� 415 kHz		100 kHz < f �� 200 kHz 
#define		EXT_OSC_FREQUENCY_RANGE4	0x04 	//415 kHz < f �� 1.1 MHz		200 kHz < f �� 400 kHz 
#define		EXT_OSC_FREQUENCY_RANGE5	0x05 	//1.1 MHz < f �� 3.1 MHz		400 kHz < f �� 800 kHz 
#define		EXT_OSC_FREQUENCY_RANGE6	0x06 	//3.1 MHz < f �� 8.2 MHz		800 kHz < f �� 1.6 MHz 
#define		EXT_OSC_FREQUENCY_RANGE7	0x07 	//8.2 MHz < f �� 25 MHz		1.6 MHz < f �� 3.2 MHz

/* ���庯������ */
extern void CloseWT(void); 							//ϵͳ���ó�ʼ��: �رտ��Ź�
extern void SysClkInit(uint8 ClkType, uint8 ClkDIV, uint8 Xoscmd, uint8 Freq);  //ϵͳʱ�ӳ�ʼ��

#endif
