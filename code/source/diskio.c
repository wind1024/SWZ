/***********************************************************************************
**�ļ�˵���� ���̲���                                                         
**�ļ����ƣ� diskio.c                                                 
**�������ڣ�2013.                                                         
**�� �� �ˣ�XZY                                                                   
**---------------------------------------------------------------------------------*
**�޸����ڣ�                                                                       
**�޸�˵����                                                                       
***********************************************************************************/
#include "memory.h"
#include "iic.h"
#include "Interrupt.h"
#define PAGINATION	0       //�洢�����޷�ҳ��0 ����ҳ��1 ��ҳ
#if	PAGINATION > 0
#define Page 128
#endif
#define MaxAddr (65536)


/***********************************************************************************/
/*�������ƣ�Uint8 DISK_Read(Uint16 Addr,Uint8 * Buf,Uint16 Count)                   */
/*�������ܣ��Ӵ洢���ж�ȡ����                                                     */
/*��ڲ�����Addr Buf Count                                                         */
/*���ز�����TRUER/FALSE                                                            */
/***********************************************************************************/
Bool DISK_Read(Uint16 Addr,Uint8 * Buf,Uint16 Count)//reentrant
{
	Uint8 i;
	i = 0;
	//GlobleIntSwitchOff();		//ȫ���жϹ�
	if((Addr+Count) > MaxAddr)       //�����洢��Χ
	{
		//GlobleIntSwitchOn();		//ȫ���жϿ�
		return FALSE;
	}
	while(!(IRcvStr(ADDR_FM24V05,Addr,Buf,Count,1)))
	{
		i ++;
		if(i > 3)
		{
			//GlobleIntSwitchOn();		//ȫ���жϿ�
			return FALSE;
		}
	}
	//GlobleIntSwitchOn();		//ȫ���жϿ�
	return TRUER;
}

/***********************************************************************************/
/*�������ƣ�Bool DISK_Write(Uint16 Addr,Uint8 * Buf,Uint16 Count)                  */
/*�������ܣ���洢����д������                                                     */
/*��ڲ�����Addr Buf Count                                                         */
/*���ز�����TRUER/FALSE                                                            */
/***********************************************************************************/
Bool DISK_Write(Uint16 Addr,Uint8 * Buf,Uint16 Count)
{
	Uint8 i;
	//GlobleIntSwitchOff();		//ȫ���жϹ�
#if	PAGINATION > 0
	Uint8 j;
#endif	
	i = 0;
	if((Addr+Count) > MaxAddr)      //�����洢��Χ
	{
		//GlobleIntSwitchOn();		//ȫ���жϿ�
		return FALSE;
	}
#if	PAGINATION > 0
	j = Page - Addr%Page;		//Addr����ҳʣ��Ŀ�����д���ֽ���
	if(Count <= j)
	{
		while(!(ISendStr(ADDR_AT24C512A,Addr,Buf,Count,1)))
		{
			i ++;
			if(i > 3)
			{
				//GlobleIntSwitchOn();		//ȫ���жϿ�
				return FALSE;
			}
		}
	}
	else
	{
		while(!(ISendStr(ADDR_AT24C512A,Addr,Buf,j,1)))       //д��Addr����ҳʣ��Ŀ�����д���ֽ���
		{
			i ++;
			if(i > 3)
			{
				//GlobleIntSwitchOn();		//ȫ���жϿ�
				return FALSE;
			}
		}
		Count = Count - j;
		Addr = Addr  + j;
		Buf = Buf + j;
		j = Count/Page;           //ʣ����ҵ��
		if(Count%Page)            //���������дһ��
		{
			j++;
		}
		for(j;j > 1;j--)                                     //д����ҵ
		{
			i = 0;
			while(!(ISendStr(ADDR_AT24C512A,Addr,Buf,Page,1)))
			{
				i ++;
				if(i > 3)
				{
					//GlobleIntSwitchOn();		//ȫ���жϿ�
					return FALSE;
				}
			}
			Addr = Addr  + Page;
			Buf = Buf + Page;
			Count = Count - Page;
		}
		while(!(ISendStr(ADDR_AT24C512A,Addr,Buf,Count,1)))  //д��ʣ�����ҵ����
		{
			i ++;
			if(i > 3)
			{
				//GlobleIntSwitchOn();		//ȫ���жϿ�
				return FALSE;
			}
		}
	}
#else
	while(!(ISendStr(ADDR_FM24V05,Addr,Buf,Count,1)))
	{
		i ++;
		if(i > 3)
		{
			//GlobleIntSwitchOn();		//ȫ���жϿ�
			return FALSE;
		}
	}	
#endif
	//GlobleIntSwitchOn();		//ȫ���жϿ�	
	return TRUER;
}
/***********************************************************************************/
/*�������ƣ�void DISK_Init(void)                                                   */
/*�������ܣ��洢����ʼ��                                                           */
/*��ڲ�������                                                                     */
/*���ز�������                                                                     */
/***********************************************************************************/
/*void DISK_Init(void)
{
//	ClearMask(AUXR,EXTRAM);					//������ѰַXRAM
	MEM_set((Uint8 *)(ADP.CALPARDat),0,sizeof(CAL_PARAMETER));
	ADP.CALPARDat ->Unit = UNIT_S_FLAG;
	ADP.CALPARDat ->SetupTimes = 5;
	ADP.CALPARDat ->RsData = 410;	   //TLC2626�����ѹ�м�ֵ
	ADP.CALPARDat ->MemInitFlag = MEM_INIT_FLAG;
	Parameter_Write(DISK_PARA_ADDR,(Uint8 *)&(ADP.CALPARDat ->UA),sizeof(CAL_PARAMETER) - sizeof(FLOAT_TEMPDAT) - 6); 
		
}*/
