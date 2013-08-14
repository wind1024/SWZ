/*
**********************************************************************************
**�ļ�˵�������ģ�⴮���ļ�                                                           
**�ļ����ƣ�SWSerial.c                                                    
**�������ڣ�2013.05.07                                                         
**�� �� �ˣ�XZY                                                                   
**---------------------------------------------------------------------------------*
**�޸����ڣ�                                                                       
**�޸�˵����                                                                       
**********************************************************************************
*/
// ��� UART ���� ʹ�� PCA ��Ϊ�����ʷ�����  
// PCA ģ�� 0 �������ղ�����Դ����ʼλ�����,Ϊ�˼����ʼλģ�� 0 ������Ϊ 
// ���ز�׽��ʽ,��������������SW_UART ����ģ�� 0 ������Ϊ�����ʱ��,ģ�� 
// ƥ���ж����ڲ���������.ģ�� 1 �������ʱ����ʽ�������Ͳ�����  
// �������һ���ⲿ���������� XTAL1 �� XTAL2 ����֮��  
// �ⲿ�����Ƶ��Ӧ�� SYSCLK �����ж���  
// �����ʿ����á���ʼλ��1λ������λ��8λ��ֹͣλ��1λ��У�飺��У��  
// ��ʼ������: 
// 1) �����ⲿ����Ƶ�ʶ��� SYSCLK  
// 2) ���� BAUD_RATE  
// 3) ���� SW_UART_INIT()  
// 4) ��λ SREN ��������� UART ������  
// 5) ��λ SES ֻ����Ҫ�û����ж�֧��ʱ  
// 6) ���� SW_UART_ENABLE()  
// 
// ����: 
// 1) ��ѯ STXBSY ���Ƿ�Ϊ 0  
// 2) д���ݵ� TDR  
// 3) ��λ CCF1 ����������  
// 4) �������ʱ��λ STI ����û����жϱ����� ����һ��EIE1.6 �Ƚ���1�ж�  
//
// ����: 
// 1) ������ڲ�ѯ��ʽ ��ѯ SRI ��������жϷ�ʽ ��EIE1.6 �Ƚ���1�жϷ�������в�ѯ SRI
// 2) �� RDR ������  
//  
 
#include "SWSerial.h"
#include "pin_define.h"

bit SRI;                           //������ɱ�־
bit STI;                           //������ɱ�־
bit STXBSY;                        //����æ��־
bit SREN;                          //��������
bit	SES;						   //SW_UART �û����ж�ʹ��λ
 
//sbit SW_RX = P1^4;                   //��������
//sbit SW_TX = P1^5;                   //��������
 
char TDR;                          //�������ݼĴ���
char RDR;                          //�������ݼĴ���

bit SW_DONE;                       //SW���ͽ�����־��������15���ַ���

//���Ա���
//char k,m;                          //���Լ�����
//char idata SW_BUF[15];             //���Խ��ջ�����
 
void PCA_ISR();                     //SW_UART�жϷ������ 

/******************************************************************************
** �� �� ��: void SW_UART_INIT(void)
** ��������: ������ڳ�ʼ��
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.05
******************************************************************************/ 
void SW_UART_INIT(void)
{
	PCA0CPM0 = 0x10;            //ģ��0Ϊ���ز�׽��ʽ����ֹģ��0�ж�										
	PCA0CPM1 = 0x48;            //ģ��1Ϊ�����ʱ����ʽ����ֹģ��1�ж�
	
	PCA0CN = 0;                 //PCA���ֽ�ֹ״̬              
	PCA0MD = 0x02;              //PCAʱ��=SYSCLK/4;��ֹPCA�������ж�
	
	CCF0 = 0;                   //���PCAģ��0��1��׽�Ƚ�ģ��
	CCF1 = 0; 
	                
	SRI = 0;                   //���������ɱ�־
	STI = 0;                    //���������ɱ�־
	
	SW_TX = 1;                  //��TX�߳�ʼ��Ϊ�ߵ�ƽ
	STXBSY = 0;                 //���SW_UARTæ��־
}
//----------------------------------------------------------------------------------
/******************************************************************************
** �� �� ��: void SW_UART_ENEABLE(void)
** ��������: �������ʹ��
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
******************************************************************************/ 
void SW_UART_ENEABLE(void)
{ 
	PCA0CPM0 |= 0x01;           //����PCAģ��0�������жϣ�
	PCA0CPM1 |= 0x01;           //����PCAģ��1�������жϣ�	
	CR = 1;                    //����PCA������
	PCA0IntSwitchOn();        //����PCA�ж�
	//GlobleIntSwitchOn();       //ȫ���ж�����
	CPT1MD = 0x10;				//�Ƚ��� 1 �½����ж����� 
	SREN = 1;					// ʹ�����ݽ���
	SES = 1;					// �û����ж�ʹ��
}
/******************************************************************************
** �� �� ��: void SW_UART_DISABLE(void)
** ��������: �ر��������
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
******************************************************************************/ 
void SW_UART_DISABLE(void)
{ 
	PCA0CPM0 &= (~0x01);           //����PCAģ��0�������жϣ�
	PCA0CPM1 &= (~0x01);           //����PCAģ��1�������жϣ�	
	CR = 0;                    //����PCA������
	PCA0IntSwitchOff();        //����PCA�ж�
	//GlobleIntSwitchOn();       //ȫ���ж�����
	CPT1MD &= (~0x10);				//�Ƚ��� 1 �½����ж����� 
	SREN = 0;					// �������ݽ���
	SES = 0;					// �û����ж�ʹ��ֹ
}
/******************************************************************************
** �� �� ��: void StartSend(void)
** ��������: ����������ڷ���
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
******************************************************************************/ 
void StartSend(void)
{
	BufDat->NumCount = 0;
	STI=1;
	SW_DONE = 0;
	CP1IntSwitchOn();   // ����Ƚ���1��CP1���ж�
	CPT1CN |= 0x10;    // �����Ƚ��� 1 �½����ж�
}
//-------------------------------------------------------------------------------------
//�жϷ������
//--------------------------------------------------------------------------------------
void PCA_ISR(void) interrupt PCA0_PRIOR_ORDER			 //11
 {
  static char SUTXST = 0;           //SW_UART TX״̬����
  static char SURXST = 0;           //SW_UART RX״̬����
  static unsigned char RXSHIFT;   //SW_UART RX��λ�Ĵ���
  static int PCA_TEMP;            //��ʱ������������ڴ���PCAģ��ĸߺ͵��ֽ�

//���ȼ������жϱ��������CCF0��λ�������� 
  if(CCF0){
     CCF0=0;                      //����жϱ�־
     switch(SURXST){
			//״̬��0 �յ���ʼλ
			//�ڸ�״̬����SW_TX�ϵĸ����ش����жϣ���ʾ��⵽��ʼλ��ͬʱPCA0CP0�Ĵ�����׽PCA0��ֵ
			//�������������ʼλ
			//��PCAģ��0�л��������ʱ��ʽ
			//��3/2λʱ�䵽ģ��0��׽�Ĵ����Բ���LSB
			//TX״̬�����1
            case 0:
                 if(SREN & ~SW_RX){  //�������������ʼλ

                    PCA_TEMP = (PCA0CPH0<<8);//��ģ��0�����ݶ���PCA_TEMP
                    PCA_TEMP |= PCA0CPL0;
                    PCA_TEMP += TH_TIME_COUNT;//��3/2λʱ�䵽PCA_TEMP

                    PCA0CPL0 = PCA_TEMP;     //����PCA0CPL0��PCA0CPH0
                    PCA0CPH0 = (PCA_TEMP>>8);

                    PCA0CPM0=0x49;         //��ģ��0�л��������ʱ����ʽ�������ж�

                    SURXST++;              //����RX״̬����  
					}              
                    break;
			//״̬1-8���յ�����λ
			//����SW_RX����
			//������������RXSHIFT
			//��1��λʱ�������ģ��0��׽�Ĵ���
			//RX״̬�����1
             case 1:             
             case 2:
             case 3:              
             case 4:            
             case 5:
             case 6: 
             case 7:
             case 8:
                 RXSHIFT = RXSHIFT >> 1; //����һλ
                 if(SW_RX)             //���SW_RX=1����1����RXSHIFT��MSB
                    RXSHIFT |= 0x80;
                   
                    PCA_TEMP = (PCA0CPH0<<8);//��ģ��0���ݶ���PCA_TEMP
                    PCA_TEMP |= PCA0CPL0;

                    PCA_TEMP += TIME_COUNT;//��1��λʱ�䵽PCA_TEMP

                    PCA0CPL0 = PCA_TEMP;   //����PCA0CPL0��PCA0CPH0
                    PCA0CPH0 = (PCA_TEMP>>8);

                    SURXST++;              //����RX״̬����
                    break;
			//״̬9�����յ�8������λ����׽ֹͣλ
			//��RXSHIFT����RDR
			//��λSRI����ʾ������ɣ�
			//����ģ��0��Ϊ��һ�δ�����׼��
			//��λRX״̬����
            case 9:
                 RDR = RXSHIFT;
                 SRI = 1;
                 PCA0CPM0 = 0x11;
                 SURXST = 0;
				 if (SES){      //����û����ж�֧�ֱ����� 
					CP1IntSwitchOn();   // ����Ƚ���1��CP1���ж�
					CPT1CN |= 0x10;    // �����Ƚ��� 1 �½����ж�
				}
                 break;
             }
 }             
//��鷢���жϣ����CCF1��λ��������
     else if(CCF1){
          CCF1=0;              //����жϱ�־
          switch(SUTXST){
		//״̬0�����͹���������
		//�ڴˣ��û��ѽ�Ҫ���͵��ֽ�װ�뵽TDR��ǿ��ģ��1�ж�����������
		//������ʼλ��ʹSW_TX��͵�ƽ��
		//��PCA0,��һ��λʱ���浽ģ��1��׽�Ĵ���
		//TX״̬������1
          case 0:
               SW_TX = 0;				  // ʹ TX ���ű����Ϊ��ʼλ 
               PCA_TEMP = PCA0L;		  // �� PCA ��������ֵ���� PCA_TEMP  
               PCA_TEMP |= (PCA0H<<8);
               PCA_TEMP += TIME_COUNT;	  // ��һ��λʱ��  
               PCA0CPL1 = PCA_TEMP;;	  // �����º��ƥ��ֵ�浽 
               PCA0CPH1 = (PCA_TEMP>>8);  // ģ�� 1 ��׽�ȽϼĴ���
               PCA0CPM1 |= 0x48;		  // ����ģ�� 1 �����ʱ��
               SUTXST++;				  // ���� TX ״̬���� 
               break;
		//״̬1-9����������λ
		//��TDR��LSB���뵽TX
		//��TDR����һλ
		//��һ��1����TDR��MSB��Ϊ״̬9��ֹͣλ
		//��һ��ʱ�䵽ģ��1��׽�Ĵ���
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 9:
               SW_TX = (TDR & 0x01);   // ��TDR��LSB�����SW_TX����  
               TDR >>= 1;			   // TDR ����һλ  
               TDR |= 0x80;			   // ��һ��'1'����TDR��MSB��Ϊ״̬9��ֹͣλ 
               PCA_TEMP = (PCA0CPH1<<8);  // ��ģ�� 0 ���ݶ���PCA_TEMP  
               PCA_TEMP |= PCA0CPL1;
               PCA_TEMP += TIME_COUNT; // ��һ��λʱ�䵽PCA_TEMP 
               PCA0CPL1 = PCA_TEMP;;   // ����ֵ�ָ� PCA0CPL1��PCA0CPH1 
               PCA0CPH1 = (PCA_TEMP>>8);
    
               SUTXST++;				 // ���� TX ״̬����
               break;
		//״̬10�����һλ�����ѷ����ꡣ����ֹͣλ�������������
		//����ֹͣλ
		//��1���ͽ�����־�����æ��־
		//��λTX״̬
		//����ģ��1��Ϊ��һ�δ�����׼��
        case 10:
              STI = 1;					 // ��ʾ�������
              SUTXST = 0;				 // ��λ TX ״̬  
              SW_TX = 1;				 // SW_TX Ӧ���ָߵ�ƽ  
              PCA0CPM1 = 0x01;			 // ��ֹģ�� 1 �����ʱ��,�����ж�Ϊ����״̬�Ա���һ�δ���
              if (SES){      			//����û����ж�֧�ֱ����� 
				CP1IntSwitchOn();   	// ����Ƚ���1��CP1���ж�
				CPT1CN |= 0x10;    		// �����Ƚ��� 1 �½����ж�  
				}
			  	STXBSY = 0;		   		// SW_UART TX ����
			  break;
            }
        }
   
}
//-------------------------------------------------------------------- 
// USER_ISR: ���Ƚ���1�ж������û� SW_UART �жϷ������(EIE1.6 �Ƚ���1�ж�) 
// ����жϲ��Է�ʽ������ �� ISR ������ 15 ���ַ������� 15 ���ַ�  
// ÿ�� SW_UART ���ͻ������ɶ�Ҫ����������  
// - ���������ָʾ��־������  
// - ��鷢�����ָʾ��־������  
// - ��� ISR ִ���ڼ��Ƿ��з��ͺͽ��չ������ ���� �ٴ���һ���ж�  
// 
void USER_ISR(void) interrupt COMPARATOR1_PRIOR_ORDER 	  // 13 �жϷ������
{     
	CPT1CN &= ~(0x10);       // ��Ƚ���1�½����жϱ�־ 
	if (SRI){         // ����������: 
		SRI = 0;         // ������ձ�־  		
		//BufDat->NumCount ++;						 //���ռ���
		if (BufDat->NumCount >= BUF_NUM){        // ��������� 
			BufDat->NumCount = 0;
			//BufDat->BitPoint = 0;
			//SREN=0;        // ��ֹ SW_UART ������ 
		}
		BufDat->Dat[BufDat->NumCount] = RDR;      // �����ջ����� 
		if((ReadMask(SysDat.Flag,ZB_ONLINE_MASK)) && (ReadMask(SysDat.Flag,SYS_HOST_MASK)))     // ZB_ONLINE_MASK AND  SYS_HOST_MASK
		{
			if((BufDat->NumCount >= 2) && (BufDat->NumCount  < DATA_LONG) && ZB_Rce_Flag )
			{
				BufDat->NumCount ++;
				if(BufDat->NumCount == DATA_LONG)
				{
					ZB_Rce_Complete = 1;
					ZB_Rce_Flag = 0;					
				}
				return;
			}
			switch(BufDat->NumCount)
				{
					case 0 :
						if(BufDat->Dat[BufDat->NumCount] == '*')
						{
							BufDat->NumCount ++;
						}
						else
						{
							BufDat->NumCount = 0;
						}
						break;
					case 1:
						if(BufDat->Dat[BufDat->NumCount] == '*')
						{
							BufDat->NumCount ++;
							ZB_Rce_Flag = 1;
						}
						else
						{
							BufDat->NumCount = 0;
						}
						break;
					default:
						break;
				}
				
		}
		else if((ReadMask(SysDat.Flag,ZB_ONLINE_MASK)) && (!ReadMask(SysDat.Flag,SYS_HOST_MASK)))
		{
			if((BufDat->NumCount >= 2) && ZB_Rce_Flag )
			{
				BufDat->NumCount ++;
				if(BufDat->NumCount == BufDat->Dat[2])
				{
					ZB_Rce_Complete = 1;
					ZB_Rce_Flag = 0;
				}
				return;
			}
			switch(BufDat->NumCount)
				{
					case 0 :
						if(BufDat->Dat[BufDat->NumCount] == '$')
						{
							BufDat->NumCount ++;
						}
						else
						{
							BufDat->NumCount = 0;
						}
						break;
					case 1:
						if(BufDat->Dat[BufDat->NumCount] == '$')
						{
							BufDat->NumCount ++;
							ZB_Rce_Flag = 1;
						}
						else
						{
							BufDat->NumCount = 0;
						}
						break;
					default:
						break;
				}
			
		}
		else
		{
			BufDat->NumCount ++;						 //���ռ���
		}
				          
	}  
	else if (STI){        // ����������: 
		STI = 0;         // ������ͱ�־  
		if (BufDat->NumCount < BufDat->BitPoint){             // ����ַ�δ������: 
			STXBSY = 1;                       // ���� SW_UART ������  
			TDR = BufDat->Dat[BufDat->NumCount++];            // ������ 1 ����
			CCF1 = 1;         // ǿ��ģ�� 1 �ж����������� 
		} 
		else
		{ 
			BufDat->NumCount = 0;
			BufDat->BitPoint = 0;
			SREN = 1;					// ʹ�����ݽ���
			SW_DONE=1;              // ��ʾ�ѷ��������һ���ַ�
		}  
	}  
//	if (STI|SRI)         // ��� SRI �� STI ��λ �ٴδ��� 
//	CPT1CN |= 0x10;    		// �����Ƚ��� 1 �½����ж� 
}
