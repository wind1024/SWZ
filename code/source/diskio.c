/***********************************************************************************
**文件说明： 磁盘操作                                                         
**文件名称： diskio.c                                                 
**创建日期：2013.                                                         
**创 建 人：XZY                                                                   
**---------------------------------------------------------------------------------*
**修改日期：                                                                       
**修改说明：                                                                       
***********************************************************************************/
#include "memory.h"
#include "iic.h"
#include "Interrupt.h"
#define PAGINATION	0       //存储器有无分页：0 不分页；1 分页
#if	PAGINATION > 0
#define Page 128
#endif
#define MaxAddr (65536)


/***********************************************************************************/
/*函数名称：Uint8 DISK_Read(Uint16 Addr,Uint8 * Buf,Uint16 Count)                   */
/*函数功能：从存储器中读取数据                                                     */
/*入口参数：Addr Buf Count                                                         */
/*返回参数：TRUER/FALSE                                                            */
/***********************************************************************************/
Bool DISK_Read(Uint16 Addr,Uint8 * Buf,Uint16 Count)//reentrant
{
	Uint8 i;
	i = 0;
	//GlobleIntSwitchOff();		//全局中断关
	if((Addr+Count) > MaxAddr)       //超出存储范围
	{
		//GlobleIntSwitchOn();		//全局中断开
		return FALSE;
	}
	while(!(IRcvStr(ADDR_FM24V05,Addr,Buf,Count,1)))
	{
		i ++;
		if(i > 3)
		{
			//GlobleIntSwitchOn();		//全局中断开
			return FALSE;
		}
	}
	//GlobleIntSwitchOn();		//全局中断开
	return TRUER;
}

/***********************************************************************************/
/*函数名称：Bool DISK_Write(Uint16 Addr,Uint8 * Buf,Uint16 Count)                  */
/*函数功能：向存储器中写入数据                                                     */
/*入口参数：Addr Buf Count                                                         */
/*返回参数：TRUER/FALSE                                                            */
/***********************************************************************************/
Bool DISK_Write(Uint16 Addr,Uint8 * Buf,Uint16 Count)
{
	Uint8 i;
	//GlobleIntSwitchOff();		//全局中断关
#if	PAGINATION > 0
	Uint8 j;
#endif	
	i = 0;
	if((Addr+Count) > MaxAddr)      //超出存储范围
	{
		//GlobleIntSwitchOn();		//全局中断开
		return FALSE;
	}
#if	PAGINATION > 0
	j = Page - Addr%Page;		//Addr所在页剩余的可连续写入字节数
	if(Count <= j)
	{
		while(!(ISendStr(ADDR_AT24C512A,Addr,Buf,Count,1)))
		{
			i ++;
			if(i > 3)
			{
				//GlobleIntSwitchOn();		//全局中断开
				return FALSE;
			}
		}
	}
	else
	{
		while(!(ISendStr(ADDR_AT24C512A,Addr,Buf,j,1)))       //写入Addr所在页剩余的可连续写入字节数
		{
			i ++;
			if(i > 3)
			{
				//GlobleIntSwitchOn();		//全局中断开
				return FALSE;
			}
		}
		Count = Count - j;
		Addr = Addr  + j;
		Buf = Buf + j;
		j = Count/Page;           //剩余整业数
		if(Count%Page)            //有余数需多写一次
		{
			j++;
		}
		for(j;j > 1;j--)                                     //写入整业
		{
			i = 0;
			while(!(ISendStr(ADDR_AT24C512A,Addr,Buf,Page,1)))
			{
				i ++;
				if(i > 3)
				{
					//GlobleIntSwitchOn();		//全局中断开
					return FALSE;
				}
			}
			Addr = Addr  + Page;
			Buf = Buf + Page;
			Count = Count - Page;
		}
		while(!(ISendStr(ADDR_AT24C512A,Addr,Buf,Count,1)))  //写入剩余非整业部分
		{
			i ++;
			if(i > 3)
			{
				//GlobleIntSwitchOn();		//全局中断开
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
			//GlobleIntSwitchOn();		//全局中断开
			return FALSE;
		}
	}	
#endif
	//GlobleIntSwitchOn();		//全局中断开	
	return TRUER;
}
/***********************************************************************************/
/*函数名称：void DISK_Init(void)                                                   */
/*函数功能：存储器初始化                                                           */
/*入口参数：无                                                                     */
/*返回参数：无                                                                     */
/***********************************************************************************/
/*void DISK_Init(void)
{
//	ClearMask(AUXR,EXTRAM);					//设置内寻址XRAM
	MEM_set((Uint8 *)(ADP.CALPARDat),0,sizeof(CAL_PARAMETER));
	ADP.CALPARDat ->Unit = UNIT_S_FLAG;
	ADP.CALPARDat ->SetupTimes = 5;
	ADP.CALPARDat ->RsData = 410;	   //TLC2626输出电压中间值
	ADP.CALPARDat ->MemInitFlag = MEM_INIT_FLAG;
	Parameter_Write(DISK_PARA_ADDR,(Uint8 *)&(ADP.CALPARDat ->UA),sizeof(CAL_PARAMETER) - sizeof(FLOAT_TEMPDAT) - 6); 
		
}*/
