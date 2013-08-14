/**************************************************************************************************
** �� �� ����C8051f93x_SMBus.h
** ��    ��: xzy    �汾: v01     ��д����: 2013.05    �������: 2013
** ��������:                       
**---------------------------------------------------------------------------------------------------
** Ӳ��ƽ̨: 
**           ML-EC5������;        																					 
** ���ƽ̨: KEIL4.10
**---------------------------------------------------------------------------------------------------
** �޸ļ�¼: 
** 			 
*****************************************************************************************************/

#ifndef C8051f93x_SMBUS_H
#define C8051f93x_SMBUS_H

#include "compiler_defs.h"             		// compiler declarations
#include "C8051F930_defs.h" 				// SFR declarations
#include "Compile.h"
#include "C8051f93x_SysClk.h"
#include "user_type.h"

#define		SMBUS_GENERATE_TIMER	3		//ѡ�ö�ʱ��2���ֽ������ΪSMBusʱ��Դ

#define		SMBus_INTERRUPT_EN		0

#define  SMB_FREQUENCY  100000          // SCLʱ������


#define  WRITE          0x00           // SMBus д����
#define  READ           0x01           // SMBus ������
                                   
// ״̬���� :SMB0CN �� 4 λ
#define  SMB_MTSTA      0xE0           // (MT) ��ʼ����
#define  SMB_MTDB       0xC0           // (MT) �����ֽڷ���
#define  SMB_MRDB       0x80           // (MR) ���ݽ���

//SMB0CF��SMBus ���üĴ���

//SMBus ʹ�ܿ��� 
#define SMBusEn()	SMB0CF |= 0x80
#define SMBusDis()	SMB0CF &= ~0x80
//SMBus �ӷ�ʽ���� 
#define SMBusSlaveModeDis()	SMB0CF |= 0x40
#define SMBusSlaveModeEn()	SMB0CF &= ~0x40

/* ���庯������ */
extern void SMBusInit(void); 						//ISMBus��ʼ��
//�����ӵ�ַ�������Ͷ��ֽ����ݺ���
extern Bool SMBusSendStr(Uint8 sla,Uint16 suba,Uint8 * s,Uint8 no,Uint8 p)reentrant;
//�����ӵ�ַ������ȡ���ֽ����ݺ���
extern Bool SMBusRcvStr(Uint8 sla,Uint16 suba,Uint8 * s,Uint8 no,Uint8 p)reentrant;


#endif