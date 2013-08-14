/*
**********************************************************************************
**�ļ�˵����ZigBee.c                                                           
**�ļ����ƣ�ZigBee����                                                    
**�������ڣ�2013.                                                         
**�� �� �ˣ�XZY                                                                   
**---------------------------------------------------------------------------------*
**�޸����ڣ�                                                                       
**�޸�˵����                                                                       
**********************************************************************************
*/
#include "AT_Commands.h"
#include "user_type.h"
#include "memory.h"
#include "SWSerial.h"
#include "SYS_ABOUT.H"
#include "pin_define.h"
#include "number_operation.h"
#include "calculate.h"
/*
*****************************************************************************
** �� �� ��: void ZigBeeNTTOAT(void)
** ��������: zigbee͸��ģʽ����ATģʽ
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/
void ZigBeeNTTOAT(void)
{
	Uint8 i,j,k;
	j = 0;
	k = 0;
	//SysDat.InterrCount = 0;	
	while(k < 3)	 //����������
	{
		for(i = 0; i < 2; i++)
		{
			MEM_cpy(BufDat->Dat,ZBSNTOATMode,3);
			BufDat->BitPoint = 3;
			SYS_Delay(200);
			StartSend();
			while(!SW_DONE);				
			SYS_Delay(200);
			SYS_Delay(200);
		//	while(SysDat.InterrCount < 2);
		//	SysDat.InterrCount = 0;	
			//SetMask(SysDat.Flag,TIME_ON_MASK);
			//while(ReadMask(SysDat.Flag,TIME_ON_MASK));
			if(MEM_cmp(&BufDat->Dat[BufDat->NumCount-4],ZBOK,4))
			{
				j++;
			}
			else
			{
				j = 0;
			}
		}
		if(j >= 2)
		{
			SysDat.Mode = ZB_AT_MODE;
			SetMask(SysDat.Flag,ZB_ONLINE_MASK);
			return;	
		}
		k++;
	}
}
/*
*****************************************************************************
** �� �� ��: Bool ZigBeeATTONT(void)
** ��������: zigbeeATģʽ����͸��ģʽ
** �䡡  ��: ��
** �䡡  ��: TRUER/FALSE
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/ 
Bool ZigBeeATTONT(void)
{
	Uint8 k;
	k = 0;
	//SysDat.InterrCount = 0;
	while(k < 3)	 //����������
	{
		MEM_cpy(BufDat->Dat,ATSTA,3);			 //AT+
		MEM_cpy(&BufDat->Dat[3],ZBESCATMode,3);//ESC
		MEM_cpy(&BufDat->Dat[6],ATEND,2);//\r\n
		BufDat->BitPoint = 8;
		//BufDat.Point = BufDat.Dat;
		//BufDat->NumCount = 0;
		SYS_Delay(50);
		StartSend();
		while(!SW_DONE);				
		SYS_Delay(200);
		SYS_Delay(200);
		//while(SysDat.InterrCount < 2);
		//SysDat.InterrCount = 0;	
		//SetMask(SysDat.Flag,TIME_ON_MASK);
		//while(ReadMask(SysDat.Flag,TIME_ON_MASK));
		if(MEM_cmp(&BufDat->Dat[BufDat->NumCount-4],ZBOK,4))
		{
			SysDat.Mode = ZB_SN_MODE;
			return TRUER;
		}
		k++;
	}
	return FALSE;
}
/*
*****************************************************************************
** �� �� ��: Bool ZigBeeAPITONT(void)
** ��������: zigbeeAPIģʽ����͸��ģʽ
** �䡡  ��: ��
** �䡡  ��: TRUER/FALSE
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/ 
Bool ZigBeeAPITONT(void)
{
	MEM_cpy(BufDat->Dat,ZBESCAPI,6);     //����͸��ģʽ
	BufDat->BitPoint = 6;
	SYS_Delay(100);
	StartSend();
	while(!SW_DONE);
	SYS_Delay(200);
	SYS_Delay(200);
	if(MEM_cmp(&BufDat->Dat[BufDat->NumCount-6],ZBAPIACK,6))
	{
		SysDat.Mode = ZB_SN_MODE;
		return TRUER;
	}
	return FALSE;
}
/*
*****************************************************************************
** �� �� ��: Bool ZigBeeNTTOAPI(void)
** ��������: zigbee͸��ģʽ����APIģʽ
** �䡡  ��: ��
** �䡡  ��: TRUER/FALSE
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/ 
Bool ZigBeeNTTOAPI(void)
{
	Uint8 i,j,k;
	k = 0;
	j = 0;
	//SysDat.InterrCount = 0;
	//BufDat.NumCount = 0;
	while(k < 3)	 //����������
	{
		for(i = 0; i < 2; i++)
		{
			MEM_cpy(BufDat->Dat,ZBSNTOAPIMode,3);      //===
			BufDat->BitPoint = 3;
			SYS_Delay(100);
			StartSend();
			while(!SW_DONE);				
			SYS_Delay(200);
			SYS_Delay(200);
		//	while(SysDat.InterrCount < 2);
		//	SysDat.InterrCount = 0;	
			//SetMask(SysDat.Flag,TIME_ON_MASK);
			//while(ReadMask(SysDat.Flag,TIME_ON_MASK));
			if(MEM_cmp(&BufDat->Dat[BufDat->NumCount-4],ZBOK,4))
			{
				j++;
			}
			else
			{
				j = 0;
			}
		}
		if(j >= 2)
		{
			SysDat.Mode = ZB_SPI_MODE;
			return TRUER;	
		}
		else
		{
			ZigBeeAPITONT();
			j = 0;
		}
		k++;
	}
	return FALSE;
}
/*
*****************************************************************************
** �� �� ��: Bool ZigBeeSETPRF(void)
** ��������: zigbeeAT���ýڵ�����������������������
** �䡡  ��: ��
** �䡡  ��: TRUER/FALSE
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/ 
Bool ZigBeeSETPRF(void)
{
	MEM_cpy(BufDat->Dat,ATSTA,3);			 //AT+
	MEM_cpy(&BufDat->Dat[3],ZBATPRF,3);	//PRF
	BufDat->Dat[6] = ATEQU[0];		   	//=
	BufDat->Dat[7] = 0x30;
	MEM_cpy(&BufDat->Dat[8],ATEND,2);	//\r\n
	BufDat->BitPoint = 10;
	SYS_Delay(50);
	StartSend();
	while(!SW_DONE);				
	SYS_Delay(100);
	SYS_Delay(100);
	SYS_Delay(100);
	SYS_Delay(100);
//	while(SysDat.InterrCount < 2);
//	SysDat.InterrCount = 0;	
	//SetMask(SysDat.Flag,TIME_ON_MASK);
	//while(ReadMask(SysDat.Flag,TIME_ON_MASK));
	if(MEM_cmp(&BufDat->Dat[BufDat->NumCount-4],ZBOK,4))
	{
		return TRUER;					
	}
	return FALSE;	
}
/*
*****************************************************************************
** �� �� ��: void ZigBeeDeviceType(void)
** ��������: ATģʽ�²�ѯZigBeeģ����豸����
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/
void ZigBeeDeviceType(void)
{
	MEM_cpy(BufDat->Dat,ATSTA,3);			 //AT+
	MEM_cpy(&BufDat->Dat[3],ZBATTYP,3);	//TYP
	BufDat->Dat[6] = ATASK[0];		   	//?
	MEM_cpy(&BufDat->Dat[7],ATEND,2);	//\r\n
	BufDat->BitPoint = 9;
	SYS_Delay(50);
	StartSend();
	while(!SW_DONE);				
	SYS_Delay(100);
	SYS_Delay(100);
	SYS_Delay(100);
	SYS_Delay(100);
	//while(SysDat.InterrCount < 2);
	//SysDat.InterrCount = 0;	
	//SetMask(SysDat.Flag,TIME_ON_MASK);
	//while(ReadMask(SysDat.Flag,TIME_ON_MASK));
	if(MEM_cmp(&BufDat->Dat[BufDat->NumCount-4],ZBOK,4))
	{
		switch(BufDat->Dat[7])
		{
			case 0x30:	 // Э����Coordinator  �����������
				SetMask(SysDat.Flag,SYS_HOST_MASK);
				break;
			case 0x31:	 // ·��Router
				ClearMask(SysDat.Flag,SYS_HOST_MASK);
				break;
			default:
				break;
		}		
	}
	
}
/*
*****************************************************************************
** �� �� ��: Bool ZigBeeSETNID(void)
** ��������: ZigBee���������ַ
** �䡡  ��: ��
** �䡡  ��: TRUER/FALSE
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/
Bool ZigBeeSETNID(void)
{
	Uint16 DriveNo = ParDat.DeviceNo;
	MEM_cpy(BufDat->Dat,ATSTA,3);			 //AT+
	MEM_cpy(&BufDat->Dat[3],ZBATNID,3);	//NID
	BufDat->Dat[6] = ATEQU[0];		   	//=
	BufDat->BitPoint = 7;
	if(DriveNo >= 10000)
	{
		NUM_ShortHexToBCD2(DriveNo,7);
		BufDat->BitPoint = BufDat->BitPoint + 5;
	}
	else if(DriveNo >= 1000)
	{
		NUM_ShortHexToBCD(DriveNo,7);
		BufDat->BitPoint = BufDat->BitPoint + 4;
	}
	else if(DriveNo >= 100)
	{
		NUM_CharHexToBCD(DriveNo,7);
		BufDat->BitPoint = BufDat->BitPoint + 3;
	}
	else if(DriveNo >= 10)
	{
		NUM_HexToBCD(DriveNo,7);
		BufDat->BitPoint = BufDat->BitPoint + 2;
	}
	else 
	{
		BufDat->Dat[7] = ((Uint8)DriveNo) | 0x30;
		BufDat->BitPoint = BufDat->BitPoint + 1;
	}
	MEM_cpy(&BufDat->Dat[BufDat->BitPoint],ATEND,2);	//\r\n
	BufDat->BitPoint = BufDat->BitPoint + 2;
	SYS_Delay(50);
	StartSend();
	while(!SW_DONE);				
	SYS_Delay(100);
	SYS_Delay(100);
	SYS_Delay(100);
	SYS_Delay(100);
	//while(SysDat.InterrCount < 2);
	//SysDat.InterrCount = 0;	
	//SetMask(SysDat.Flag,TIME_ON_MASK);
	//while(ReadMask(SysDat.Flag,TIME_ON_MASK));
	if(MEM_cmp(&BufDat->Dat[BufDat->NumCount-4],ZBOK,4))
	{
		return TRUER;					
	}
	return FALSE;	
}
/*
*****************************************************************************
** �� �� ��: Bool ZigBeeSRS(void)
** ��������: ZigBee����
** �䡡  ��: ��
** �䡡  ��: TRUER/FALSE
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/
Bool ZigBeeSRS(void)
{
	MEM_cpy(BufDat->Dat,ATSTA,3);			 //AT+
	MEM_cpy(&BufDat->Dat[3],ZBATSRS,3);	//SAV
	MEM_cpy(&BufDat->Dat[6],ATEND,2);	//\r\n
	BufDat->BitPoint = 8;
	//BufDat->NumCount = 0;
	SYS_Delay(50);
	StartSend();
	while(!SW_DONE);				
	SYS_Delay(200);
	SYS_Delay(200);
	SYS_Delay(200);
	SYS_Delay(200);
	//while(SysDat.InterrCount < 2);
	//SysDat.InterrCount = 0;	
	//SetMask(SysDat.Flag,TIME_ON_MASK);
	//while(ReadMask(SysDat.Flag,TIME_ON_MASK));
	if(MEM_cmp(&BufDat->Dat[BufDat->NumCount-4],ZBOK,4))
	{
		return TRUER;					
	}
	return FALSE;	
}
/*
*****************************************************************************
** �� �� ��: Bool ZigBeeSAVDat(void)
** ��������: ZigBee��������     ����1s
** �䡡  ��: ��
** �䡡  ��: TRUER/FALSE
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/
Bool ZigBeeSAVDat(void)
{
	MEM_cpy(BufDat->Dat,ATSTA,3);			 //AT+
	MEM_cpy(&BufDat->Dat[3],ZBATSAV,3);	//SAV
	MEM_cpy(&BufDat->Dat[6],ATEND,2);	//\r\n
	BufDat->BitPoint = 8;
	//BufDat->NumCount = 0;
	SYS_Delay(50);
	StartSend();
	while(!SW_DONE);				
	SYS_Delay(100);
	SYS_Delay(200);
	SYS_Delay(100);
	SYS_Delay(200);
	SYS_Delay(100);
	SYS_Delay(200);
	SYS_Delay(100);
	SYS_Delay(200);
	if(MEM_cmp(&BufDat->Dat[BufDat->NumCount-4],ZBOK,4))
	{
		return TRUER;					
	}
	return FALSE;	
}

/*
*****************************************************************************
**�� �� ����ZigBeeSend(Uint8 *p,Uint16 data_len);
**����������ZigBeeģ�鷢������
**��    �룺Uint8 *p,Uint16 data_len
**��    ����
*****************************************************************************
*/
void ZigBeeSend(Uint8 *p,Uint16 data_len)
{
	//unsigned int xdata buffer_len=0;
	BufDat->BitPoint = 0;
	if(ReadMask(SysDat.Flag,SYS_HOST_MASK) == SYS_HOST_MASK)     //Э�� ΪAPIģʽ
	{
		if(ZigBeeNTTOAPI() == TRUER)                             //��APIģʽ
		{
			BufDat->Dat[BufDat->BitPoint++] = 0xFE;
			BufDat->Dat[BufDat->BitPoint++] = data_len + 2;
			BufDat->Dat[BufDat->BitPoint++] = 0x24;
			BufDat->Dat[BufDat->BitPoint++] = 0x5F;
			BufDat->Dat[BufDat->BitPoint++] = *(p+4);      //Ŀ���ַ
			BufDat->Dat[BufDat->BitPoint++] = *(p+3);
			MEM_cpy((Uint8 *)&BufDat->Dat[BufDat->BitPoint],p,data_len);
			BufDat->BitPoint = BufDat->BitPoint + data_len;
			BufDat->Dat[BufDat->BitPoint++] = MsgCheckSum((Uint8 *)&BufDat->Dat[1],BufDat->BitPoint-1);
			SYS_Delay(50);
			StartSend();
			while(!SW_DONE);
			SYS_Delay(200);
			SYS_Delay(200);
			if(MEM_cmp(BufDat->Dat,ZBAPIDATOK,6))
			{
				ZigBeeAPITONT();
				//SREN=0;        // ��ֹ SW_UART ������ 
			}
		}
	}
	else if(ReadMask(SysDat.Flag,SYS_HOST_MASK) != SYS_HOST_MASK) //·�� Ϊ͸��ģʽ
	{
		MEM_cpy(BufDat->Dat,p,data_len);
		BufDat->BitPoint = data_len;
		SYS_Delay(50);
		StartSend();
		while(!SW_DONE);		
	}
}
/*
*****************************************************************************
** �� �� ��: void ZigBeeInit(void)
** ��������: ZigBeeģ���ʼ��
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/ 
void ZigBeeInit(void)
{
	//ZigBeeSETNID();
	ZB_Rce_Flag = 0;
	ZB_Rce_Complete = 0;
	MEM_set(BufDat->Dat,0,sizeof(BUF_DAT));
	//ZigBeeNTTOAPI();
	//ZigBeeAPITONT();
	ZigBeeNTTOAT();
	if(ReadMask(SysDat.Flag,ZB_ONLINE_MASK) == ZB_ONLINE_MASK)
	{
		ClearMask(SysDat.Flag,ZB_ONLINE_MASK);       //�������־������������
		ZigBeeSETPRF();
		ZigBeeDeviceType();
		if(ReadMask(SysDat.Flag,SYS_HOST_MASK) != SYS_HOST_MASK)  //·��Router,Э����Coordinator ����Ҫ����
		{			
			ParDat.DeviceNo ++;
			ZigBeeSETNID();
			ZigBeeSAVDat();
		}
		//ZigBeeATTONT();		  
		ZigBeeSRS();          //��Ӳ����λ��Чʱ������ɲ�ִ��
		ZIGBEE_RSTON();		  //Ӳ����λ
		SYS_Delay(200);
		SYS_Delay(200);
		//SetMask(SysDat.Flag,TIME_ON_MASK);
		//while(ReadMask(SysDat.Flag,TIME_ON_MASK));
		ZIGBEE_RSTOFF();
		SetMask(SysDat.Flag,ZB_ONLINE_MASK);    //����������ɣ���ZigBee���߱�־
		SYS_Delay(200);
		SYS_Delay(200);
		SYS_Delay(200);
		SYS_Delay(200);
	}
	MEM_set(BufDat->Dat,0,sizeof(BUF_DAT));

}