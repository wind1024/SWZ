/*
**********************************************************************************
**文件说明：ZigBee.c                                                           
**文件名称：ZigBee操作                                                    
**创建日期：2013.                                                         
**创 建 人：XZY                                                                   
**---------------------------------------------------------------------------------*
**修改日期：                                                                       
**修改说明：                                                                       
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
** 函 数 名: void ZigBeeNTTOAT(void)
** 功能描述: zigbee透传模式进入AT模式
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/
void ZigBeeNTTOAT(void)
{
	Uint8 i,j,k;
	j = 0;
	k = 0;
	//SysDat.InterrCount = 0;	
	while(k < 3)	 //最多测试三次
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
** 函 数 名: Bool ZigBeeATTONT(void)
** 功能描述: zigbeeAT模式进入透传模式
** 输　  入: 无
** 输　  出: TRUER/FALSE
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/ 
Bool ZigBeeATTONT(void)
{
	Uint8 k;
	k = 0;
	//SysDat.InterrCount = 0;
	while(k < 3)	 //最多测试三次
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
** 函 数 名: Bool ZigBeeAPITONT(void)
** 功能描述: zigbeeAPI模式进入透传模式
** 输　  入: 无
** 输　  出: TRUER/FALSE
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/ 
Bool ZigBeeAPITONT(void)
{
	MEM_cpy(BufDat->Dat,ZBESCAPI,6);     //返回透传模式
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
** 函 数 名: Bool ZigBeeNTTOAPI(void)
** 功能描述: zigbee透传模式进入API模式
** 输　  入: 无
** 输　  出: TRUER/FALSE
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/ 
Bool ZigBeeNTTOAPI(void)
{
	Uint8 i,j,k;
	k = 0;
	j = 0;
	//SysDat.InterrCount = 0;
	//BufDat.NumCount = 0;
	while(k < 3)	 //最多测试三次
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
** 函 数 名: Bool ZigBeeSETPRF(void)
** 功能描述: zigbeeAT设置节点接收无线数据输出串口命令
** 输　  入: 无
** 输　  出: TRUER/FALSE
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
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
** 函 数 名: void ZigBeeDeviceType(void)
** 功能描述: AT模式下查询ZigBee模块的设备类型
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
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
			case 0x30:	 // 协调器Coordinator  即网络的中心
				SetMask(SysDat.Flag,SYS_HOST_MASK);
				break;
			case 0x31:	 // 路由Router
				ClearMask(SysDat.Flag,SYS_HOST_MASK);
				break;
			default:
				break;
		}		
	}
	
}
/*
*****************************************************************************
** 函 数 名: Bool ZigBeeSETNID(void)
** 功能描述: ZigBee设置网络地址
** 输　  入: 无
** 输　  出: TRUER/FALSE
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
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
** 函 数 名: Bool ZigBeeSRS(void)
** 功能描述: ZigBee从启
** 输　  入: 无
** 输　  出: TRUER/FALSE
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
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
** 函 数 名: Bool ZigBeeSAVDat(void)
** 功能描述: ZigBee保存设置     返回1s
** 输　  入: 无
** 输　  出: TRUER/FALSE
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
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
**函 数 名：ZigBeeSend(Uint8 *p,Uint16 data_len);
**功能描述：ZigBee模块发送数据
**输    入：Uint8 *p,Uint16 data_len
**输    出：
*****************************************************************************
*/
void ZigBeeSend(Uint8 *p,Uint16 data_len)
{
	//unsigned int xdata buffer_len=0;
	BufDat->BitPoint = 0;
	if(ReadMask(SysDat.Flag,SYS_HOST_MASK) == SYS_HOST_MASK)     //协调 为API模式
	{
		if(ZigBeeNTTOAPI() == TRUER)                             //进API模式
		{
			BufDat->Dat[BufDat->BitPoint++] = 0xFE;
			BufDat->Dat[BufDat->BitPoint++] = data_len + 2;
			BufDat->Dat[BufDat->BitPoint++] = 0x24;
			BufDat->Dat[BufDat->BitPoint++] = 0x5F;
			BufDat->Dat[BufDat->BitPoint++] = *(p+4);      //目标地址
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
				//SREN=0;        // 禁止 SW_UART 接收器 
			}
		}
	}
	else if(ReadMask(SysDat.Flag,SYS_HOST_MASK) != SYS_HOST_MASK) //路由 为透传模式
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
** 函 数 名: void ZigBeeInit(void)
** 功能描述: ZigBee模块初始化
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
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
		ClearMask(SysDat.Flag,ZB_ONLINE_MASK);       //先清除标志，做后续设置
		ZigBeeSETPRF();
		ZigBeeDeviceType();
		if(ReadMask(SysDat.Flag,SYS_HOST_MASK) != SYS_HOST_MASK)  //路由Router,协调器Coordinator 不需要设置
		{			
			ParDat.DeviceNo ++;
			ZigBeeSETNID();
			ZigBeeSAVDat();
		}
		//ZigBeeATTONT();		  
		ZigBeeSRS();          //当硬件复位有效时此命令可不执行
		ZIGBEE_RSTON();		  //硬件复位
		SYS_Delay(200);
		SYS_Delay(200);
		//SetMask(SysDat.Flag,TIME_ON_MASK);
		//while(ReadMask(SysDat.Flag,TIME_ON_MASK));
		ZIGBEE_RSTOFF();
		SetMask(SysDat.Flag,ZB_ONLINE_MASK);    //所有设置完成，置ZigBee在线标志
		SYS_Delay(200);
		SYS_Delay(200);
		SYS_Delay(200);
		SYS_Delay(200);
	}
	MEM_set(BufDat->Dat,0,sizeof(BUF_DAT));

}