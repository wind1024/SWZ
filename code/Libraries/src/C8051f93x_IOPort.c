/**************************************************************************************************
** �� �� ����C8051f93x_IOPort.c
** ��    ��: xzy    �汾: v01     ��д����: 2013    �������: 2013
** ��������:1. IO�ڳ�ʼ������
**           2. ʹ�ú궨�庯��IO_PORT_PIN_MODE(port, pin, mode,outmode)��ʼ��ĳһ���Ź���ģʽ
**           3. ʹ�ú궨�庯��IO_PORT_MODE(port, mode,outmode)��ʼ��ĳһ�˿ڹ���ģʽ
**           4. ʹ�ú궨�庯��PortUART0En()��ʹ����������Ľ��濪��
**           5. ֻ��P1��(ADC2)�ܳ�ʼ��Ϊģ������ģʽ
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
#include 	"C8051f93x_IOPort.h"

/******************************************************************************
** �� �� ��: IOPortInit
** ��������: IO�ڳ�ʼ��
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** ��д����: 2013-04-25
** �������: 2013-04-25
******************************************************************************/ 
void IOPortInit(void)
{
	// vref agnd xtal1 xtal2 ���濪������P0.0~P0.3
	IO_PORT_PIN_SKIP(0,0);
	IO_PORT_PIN_SKIP(0,1);
	IO_PORT_PIN_SKIP(0,2);
	IO_PORT_PIN_SKIP(0,3);
	// P0.0~P0.3����Ϊģ�ⷽʽ
	IO_PORT_PIN_MODE(0,0,ANALOG_INPUT,NULL);
	IO_PORT_PIN_MODE(0,1,ANALOG_INPUT,NULL);
	IO_PORT_PIN_MODE(0,2,ANALOG_INPUT,NULL);
	IO_PORT_PIN_MODE(0,3,ANALOG_INPUT,NULL);
	// SDA�� SCL �����˿�����
	IO_PORT_PIN_SKIP(0,6);
	IO_PORT_PIN_SKIP(0,7);
	IO_PORT_PIN_MODE(0,6,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(0,7,DIGITAL_INPUT,PUSH_PULL_OUT);
	//PortSMB0En();
	//TX0 ��RX0 �����˿����� P0.4��P0.5
	PortUART0En();
	// Set UART0 TX pin to push-pull 
	IO_PORT_PIN_MODE(0,4,DIGITAL_INPUT,PUSH_PULL_OUT);
	// ���濪������P1.0~P1.3��P1.6��P1.7
	IO_PORT_PIN_SKIP(1,0);
	IO_PORT_PIN_SKIP(1,1);
	IO_PORT_PIN_SKIP(1,2);
	IO_PORT_PIN_SKIP(1,3);
	//IO_PORT_PIN_SKIP(1,5);
	IO_PORT_PIN_SKIP(1,6);
	IO_PORT_PIN_SKIP(1,7);
	// p1.0 - p1.1 ad pin ģ�ⷽʽ
	IO_PORT_PIN_MODE(1,0,ANALOG_INPUT,NULL);
	IO_PORT_PIN_MODE(1,1,ANALOG_INPUT,NULL);
	//P1.2,P1.3,P1.6 ,P1.7 �������
	IO_PORT_PIN_MODE(1,2,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(1,3,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(1,6,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(1,7,DIGITAL_INPUT,PUSH_PULL_OUT);
	//IO_PORT_PIN_MODE(1,2,ANALOG_INPUT,NULL);
	//IO_PORT_PIN_MODE(1,3,ANALOG_INPUT,NULL);	
	// cex0 �����˿�����P1.4
	PortPCA0M0En();
	// Set SOFT UART TX pin to push-pull 
	IO_PORT_PIN_MODE(1,5,DIGITAL_INPUT,PUSH_PULL_OUT);
	//PortCP1En();
	// P2.0~P2.6 �������
	IO_PORT_PIN_MODE(2,0,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,1,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,2,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,3,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,4,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,5,DIGITAL_INPUT,PUSH_PULL_OUT);
	IO_PORT_PIN_MODE(2,6,DIGITAL_INPUT,PUSH_PULL_OUT);
	
	GlobalWeakPullEn();						//ʹ�����ŵ�ȫ������������
	CrossBarEn();							//���濪��ʹ��

	//���ܽ��濪���Ƿ�һ���˿����ŷ����ĳ���������裬�˿����ŵ������ʽ����PnMDOUT�Ĵ������ơ�

}
