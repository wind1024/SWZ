/*
*************************************************************************************************
** �� �� ����C8051f93x_UART0.h
** ��    ��: xzy    �汾: v01     ��д����: 2013.04.27    �������: 2013
** ��������:          
**---------------------------------------------------------------------------------------------------
** Ӳ��ƽ̨: 
**           ML-EC5������;        																					 
** ���ƽ̨: KEIL4.10
**---------------------------------------------------------------------------------------------------
** �޸ļ�¼: 
** 			 
****************************************************************************************************
*/
#ifndef 	C8051f93x_UART0_H
#define 	C8051f93x_UART0_H

#include "compiler_defs.h"             		// compiler declarations
#include "C8051F930_defs.h" 				// SFR declarations
#include "Compile.h"
#include "C8051f93x_SysClk.h"	
#include "C8051f93x_Timer01.h"

#define		BAUD_GENERATE_TIMER1	1		//ѡ�ö�ʱ��1��Ϊ�����ʷ�����

#define		UART0_INTERRUPT_EN		1       //�жϷ������

#if	UART0_INTERRUPT_EN > 0
//extern	uint8	UART0RecvBuf;
//extern	uint8	UART0RecvIntFlag;
#endif

/* �����ʺ궨�� */

#define 	BAUDRATE           115200         // ������ >=1200	��߲��Թ�115200

//#define		UART0_BAUD_DOUBLE		0			//�Ƿ���Ҫ�����ʼӱ�

/* ���ڷ�ʽ�궨�� */
#define		UART0_MODE1				0			//��ʽ1��8 λUART���ɱ䲨����
#define		UART0_MODE3				1			//��ʽ3��9 λUART���ɱ䲨����
//�ദ����ͨ�ſ���
#define		UART0MCEEN			MCE0 = 1 
#define		UART0MCEDIS			MCE0 = 0
//���տ���
//#define		UART0RECVEN			 REN0 = 1
//#define		UART0RECVDIS		 REN0 = 0

/* ���庯������ */
void UART0Init(uint8 UART0Mode);
//extern void UART0SendByte(uint8 Val);			//ͨ��UART0����һ���ֽ�
//extern void UART0_ReadyRecv(void);				//UART0׼���������ֽ�
//extern uint8 UART0_RecvNewDat(void);			//�Ƿ���������
//extern uint8 UART0_RecvOneDat(void);			//����һ���ֽ�����



#endif