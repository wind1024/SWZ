/*
**********************************************************************************
**�ļ�˵����SWZ������                                                               
**�ļ����ƣ�main.c                                                      
**�������ڣ�2013.05.24                                                            
**�� �� �ˣ�XZY                                                                   
**---------------------------------------------------------------------------------*
**�޸����ڣ�                                                                       
**�޸�˵����                                                                       
**********************************************************************************
*/

/*
-------------------------------------------------------------------------------------
˵��:include 
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
		if((ReadMask(SysDat.Flag,SYS_HOST_MASK)== SYS_HOST_MASK)&&(ReadMask(SysDat.Flag,ZB_ONLINE_MASK) == ZB_ONLINE_MASK))       //����ZigBee����
		{			
			RouterDatProcess();
			PCON |= 0x01;                              //���з�ʽ
		}
	}
	
}