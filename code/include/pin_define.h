/***********************************************************************************
**�ļ�˵����MCU�ܽŹ��ܱ�������                                                            
**�ļ����ƣ�pin.define.h                                                       
**�������ڣ�2013.                                                         
**�� �� �ˣ�XZY                                                                   
**---------------------------------------------------------------------------------*
**�޸����ڣ�                                                                       
**�޸�˵����                                                                       
***********************************************************************************/

#ifndef PIN_DEFINE
#define PIN_DEFINE

#include <compiler_defs.h>             		// compiler declarations
#include <C8051F930_defs.h> 				// SFR declarations



	
sbit SDA		= P0^6;			//ģ��I2C���ݴ���λ
sbit SCL		= P0^7;			//ģ��I2Cʱ�ӿ���λ

sbit DATA_OK	= P1^2;			//���ٶ����ݾ���
sbit CHARGE_C	= P1^3;			//������

sbit SW_RX		= P1^4;  		// SW_UART Receive pin������ڽ�������
sbit SW_TX		= P1^5;  		// SW_UART Transmit pin������ڷ�������

sbit ZIGBEE_RST	= P1^6;			//ZigBee��λ����
sbit G_POWER	= P1^7;			//���ٶ�ģ���Դ

sbit PC_SET		= P2^0;			//�������� ,����ģʽ
sbit GSM_REST	= P2^1;			//GSM/GPRSģ�鸴λ
sbit GSM_RTS	= P2^2;			//GSM/GPRSģ��RTS�ź�
sbit GSM_CTS	= P2^3;			//GSM/GPRSģ��CTS�ź�
//sbit GSM_RESET	= P2^4;			//GSM/GPRSģ�鸴λ
sbit USER_LED	= P2^4;			//
sbit SYS_POWER	= P2^6;			//ϵͳ��Դ����
#endif
