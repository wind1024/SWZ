/*
**********************************************************************************
**文件说明：SWZ主函数                                                               
**文件名称：main.c                                                      
**创建日期：2013.05.24                                                            
**创 建 人：XZY                                                                   
**---------------------------------------------------------------------------------*
**修改日期：                                                                       
**修改说明：                                                                       
**********************************************************************************
*/

/*
-------------------------------------------------------------------------------------
说明:include 
-------------------------------------------------------------------------------------
*/
#include "SYS_ABOUT.H"
#include "pin_define.h"
#include "memory.h"
void main()
{
	SYS_Init();
	G_POWEROFF();
	POWEROFF();
	ZIGBEE_RSTOFF();
	InitVariable();
	USER_LED = 0;
	while(1)
	{
		SysDat.MainLoopError = 0;		
		if(ReadMask(SysDat.Flag,SYS_BE_WORKING) == SYS_BE_WORKING)
		{
			SYS_Daemon();
			//ClearMask(SysDat.Flag,SYS_BE_WORKING);
			Sleep();	
		}
		if((ReadMask(SysDat.Flag,SYS_HOST_MASK)== SYS_HOST_MASK)&&(ReadMask(SysDat.Flag,ZB_ONLINE_MASK) == ZB_ONLINE_MASK))       //主机ZigBee在线
		{			
			RouterDatProcess();
			PCON |= 0x01;                              //空闲方式
		}
	}
	
}