/***********************************************************************************
**�ļ�˵����C8051f93xʱ��ϵͳ����                                                              
**�ļ����ƣ�C8051f93x_SysClk.c                                                      
**�������ڣ�2013.04                                                            
**�� �� �ˣ�XZY                                                                   
**---------------------------------------------------------------------------------*
**�޸����ڣ�                                                                       
**�޸�˵����                                                                       
***********************************************************************************/

#include "C8051f93x_SysClk.h"

/******************************************************************************
** �� �� ��: void CloseWT(void)
** ��������: ϵͳ���ó�ʼ��: �رտ��Ź�
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.04
******************************************************************************/ 
void CloseWT(void)
{
   PCA0MD &= ~0x40;                    		// WDTE = 0 ��ֹ���Ź���ʱ��
}

/******************************************************************************
** �� �� ��: SysClkInit
** ��������: ϵͳʱ�ӳ�ʼ��
**           
** �䡡  ��: uint8 ClkType, uint8 ClkDIV, uint8 Xoscmd, uint8 Freq
**			ClkType:  	CLK_INT_OSC		ʹ�þ�׼�ڲ�����ʱ��
**                     	CLK_EXT_OSC		ʹ���ⲿ��������ʱ��
**					   	CLK_smaRTClock_OSC	ʹ��smaRTClock����ʱ��
**					   	CLK_LOW_POWER_OSC	ʹ�õ͹����ڲ�����ʱ��
**			ClkDIV:(��Ƶϵ��)
**						OSC_SYSCLK_Div1
**						OSC_SYSCLK_Div2
**						OSC_SYSCLK_Div4
**						OSC_SYSCLK_Div8
**						OSC_SYSCLK_Div16
**						OSC_SYSCLK_Div32
**						OSC_SYSCLK_Div64
**						OSC_SYSCLK_Div128
**			Xoscmd: (�ⲿ������ʽ)
**                     	EXT_OSC_DISABLE		�ⲿ������·��ֹ
** 						EXT_OSC_CMOS	 	�ⲿCMOSʱ�ӷ�ʽ
** 						EXT_OSC_CMOS_Div2 	�ⲿCMOSʱ�ӷ�ʽ����Ƶ
** 						EXT_OSC_RC		 	�ⲿRC������ʽ
**						EXT_OSC_C		 	�ⲿ����������ʽ
** 						EXT_OSC_CRY	 		�ⲿ����������ʽ
** 						EXT_OSC_CRY_Div2	�ⲿ����������ʽ����Ƶ
**			Freq�����ⲿ����Ƶ�ʿ���XFCN��(���ѡ���ⲿ����)	   	
**                 		 							   ����Ƶ��					��RC�� C��ʽ�� 
** 						EXT_OSC_FREQUENCY_RANGE0		f �� 20kHz 			    f �� 25 kHz 
** 						EXT_OSC_FREQUENCY_RANGE1	20 kHz < f �� 58 kHz			25 kHz < f �� 50 kHz 
** 						EXT_OSC_FREQUENCY_RANGE2 	58 kHz < f �� 155 kHz		50 kHz < f �� 100 kHz 
** 						EXT_OSC_FREQUENCY_RANGE3	155 kHz < f �� 415 kHz		100 kHz < f �� 200 kHz 
** 						EXT_OSC_FREQUENCY_RANGE4	415 kHz < f �� 1.1 MHz		200 kHz < f �� 400 kHz 
**						EXT_OSC_FREQUENCY_RANGE5	1.1 MHz < f �� 3.1 MHz		400 kHz < f �� 800 kHz 
**						EXT_OSC_FREQUENCY_RANGE6	3.1 MHz < f �� 8.2 MHz		800 kHz < f �� 1.6 MHz 
**						EXT_OSC_FREQUENCY_RANGE7	8.2 MHz < f �� 25 MHz		1.6 MHz < f �� 3.2 MHz
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.04
******************************************************************************/ 
void SysClkInit(uint8 ClkType, uint8 ClkDIV, uint8 Xoscmd, uint8 Freq)	
{
	//uint16 i = 0;

   //uint8 SFRPAGE_SAVE = SFRPAGE;    	// ���浱ǰSFRPAGE
   //SFRPAGE = CONFIG_PAGE;              	// �ı�ҳ

   switch(ClkType)
   {
		case CLK_INT_OSC:				//ʹ���ڲ�����ʱ��(�ϵ縴λ���ʹ�����ַ�ʽ)
			//�ı�ʱ��Ƶ��ʱ,���л����ڲ��͹�������,��֤ϵͳ��������
			//CLKSEL = 0x40;							
			OSCICN = OSCICN | 0x80;			
			while((OSCICN & 0x40) == 0);//�ȴ��ڲ�����Ƶ��׼���ñ�־
			CLKSEL = 0x00 | ClkDIV;  	//��ϵͳʱ��Դ�л����ڲ�����
			while((CLKSEL & 0x80) == 0);//�ȴ�ʱ�ӷ�Ƶ����Ӧ����ϵͳʱ��
			break;
		case CLK_EXT_OSC:			//ʹ���ⲿ��������ʱ��
			//�ı�ʱ��Ƶ��ʱ,���л����ڲ��͹�������,��֤ϵͳ��������
			//CLKSEL = 0x40;
			OSCICN = OSCICN | 0x80;			
			//�����ⲿʱ��
			OSCXCN |= (Xoscmd | Freq);		// ����������ʽ,�ⲿ����ƫ�õ���
			if((Xoscmd != EXT_OSC_CMOS )&&(Xoscmd != EXT_OSC_CMOS_Div2) )
			{
				while(!(OSCXCN & 0x80));		// �ȴ����������ȶ�����
			}
			CLKSEL = 0x01 | ClkDIV;  	  // ѡ���ⲿ������ϵͳʱ�ӷ�Ƶ 
			while((CLKSEL & 0x80) == 0);//�ȴ�ʱ�ӷ�Ƶ����Ӧ����ϵͳʱ��
			OSCICN &= ~0x80;			  // ��ֹ�ڲ�����
			break;
		case CLK_smaRTClock_OSC:	//ʹ��smaRTClock����ʱ��			
			break;
		case CLK_LOW_POWER_OSC:	    //ʹ�õ͹����ڲ�����ʱ��
			//CLKSEL = 0x40;
			break;
		default:		//ʹ���ڲ�����ʱ��(�ϵ縴λ���ʹ�����ַ�ʽ)
			//CLKSEL = 0x40;
			OSCICN = OSCICN | 0x80;
			break;
   }
   	RSTSRC = 0x06;                      			// ʹ��ʱ�Ӷ�ʧ�������MCD��
                                       				// ʹ��VDD/DC+��Դ������Ϊ��λԴ
}


