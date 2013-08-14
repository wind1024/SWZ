/**************************************************************************************************
** 文 件 名：C8051f93x_SMBus.c
** 作    者: xzy    版本: v01     编写日期: 2013.05    完成日期: 2013
** 功能描述:                       
**---------------------------------------------------------------------------------------------------
** 硬件平台: 
**           ML-EC5仿真器;        																					 
** 软件平台: KEIL4.10
**---------------------------------------------------------------------------------------------------
** 修改记录: 
** 			 
*****************************************************************************************************/

#include "C8051f93x_SysClk.h"
#include "C8051f93x_Timer23.h"
#include "C8051f93x_SMBus.h"

/******************************************************************************
** 函 数 名: void SMBusInit(void)
** 功能描述: SMBus初始化
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.05
******************************************************************************/ 
void SMBusInit(void)
{
	uint8 	i;
	SMB0CF = 0;	  //清SMBus 配置寄存器
	SMB0CF |= SMBUS_GENERATE_TIMER;	//SMBus 时钟源选择
	SMBusSlaveModeDis();		    //SMBus 从方式禁止
#if ((SYSCLK/SMB_FREQUENCY/3) < 255)     
	  CKCON |= 0x30;                   // 定时器2 高低字节时钟 = SYSCLK
	  i = -(SYSCLK/SMB_FREQUENCY/3);   //定时器2溢出率为SCL时钟速率的1/3
#elif ((SYSCLK/SMB_FREQUENCY/12/3) < 255)
      CKCON &= ~0x30;                  //定时器2 高低字节时钟 = SYSCLK/12
	  TMR2CN &= ~0x03;				   
	  i = -(SYSCLK/SMB_FREQUENCY/12/3);   //定时器2溢出率为SCL时钟速率的1/3
#endif
	TMR2CN |= 0x08;		//定时器2工作在双 8 位自动重装载定时器方式
	TMR2RLL = i;
	TMR2RLH = TMR2RLL;
	TMR2L	= TMR2RLL;
	TMR2H	= TMR2RLL;
	Timer2Double8bitEN();		
	Timer2Open();
	SMBusEn();
	STO = 1;                //结束总线	
}
/******************************************************************************
** 函 数 名:Bool SMBusSendStr(Uint8 sla,Uint16 suba,Uint8 * s,Uint8 no,Uint8 p)reentrant 
** 功能描述: 向有子地址器件发送多字节数据函数
**			从启动总线到发送地址，子地址,数据，结束总线的全过程.
**	注意：   使用前必须已结束总线。 
** 输　  入: 从器件地址sla，子地址suba，发送内容是s指向的内容，p发送子地址            
**           8位或16位选择，发送no个字节。				         
** 输　  出: 1表示操作成功，否则操作有误。
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.05
******************************************************************************/ 
Bool SMBusSendStr(Uint8 sla,Uint16 suba,Uint8 * s,Uint8 no,Uint8 p)reentrant
{
    Uint8 i;
	SI = 0;
	STO = 0;  
    STA = 1;                //启动总线
    while(!SI);
	STA = 0; 
	SMB0DAT= sla;              //发送器件地址
	SI = 0;
	while(!SI);
    //if(ACK==0)return(0);
    if(p){                      //发送子地址
        SMB0DAT = (suba/256);        
        SI = 0;
		while(!SI);
		//if(ACK==0)return(0);
        SMB0DAT = (suba%256);
		SI = 0;
		while(!SI); 
        //if(ACK==0)return(0);    
    }
    else{
        SMB0DAT = ((Uint8) suba);
		SI = 0;
		while(!SI);
        //if(ACK==0)return(0);
    }
    for(i=0;i<no;i++){   
		SMB0DAT = (*s);            //发送数据
		SI = 0;
		while(!SI);
	  //if(ACK==0)return(0);
		s++;
    } 
    STO = 1;                //结束总线
    return(1);	   
}

/******************************************************************************
** 函 数 名: Bool SMBusRcvStr(Uint8 sla,Uint16 suba,Uint8 * s,Uint8 no,Uint8 p)reentrant 
** 功能描述: 向有子地址器件读取多字节数据函数
**			从启动总线到发送地址，子地址,读数据，结束总线的全过程.                                         								   
**注意：    使用前必须已结束总线。                                                    
** 输　  入: 从器件地址sla，子地址suba，读出的内容放入s指向的存储区，p发送子地址           
**           8位或16位选择，读no个字节。                 
** 输　  出: 1表示操作成功，否则操作有误。
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.05
******************************************************************************/ 
Bool SMBusRcvStr(Uint8 sla,Uint16 suba,Uint8 * s,Uint8 no,Uint8 p)reentrant 
{
    Uint8 i;
    SI = 0;
	STO = 0;
	STA = 1;                    //启动总线
   	while(!SI);
	STA = 0; 
    SMB0DAT = sla;                  //发送器件地址
    SI = 0;
	while(!SI);
	//if(ACK==0)return(0);
    if(p){                           //发送子地址
        SMB0DAT = (suba/256);        
        SI = 0;
		while(!SI);
		//if(ACK==0)return(0);
        SMB0DAT = (suba%256); 
        SI = 0;
		while(!SI);
		//if(ACK==0)return(0);    
    }
    else{
        SMB0DAT = ((Uint8) suba);
       	SI = 0;
		while(!SI);
	    //if(ACK==0)return(0);
    }
    STA = 1;                  //重新启动总线
	while(!SI);
	STA = 0;
	SMB0DAT = (sla+1);
    SI = 0;
	while(!SI);
	//if(ACK==0)return(0);
    for(i=0;i<no-1;i++){   
       	SI = 0;
		while(!SI);
	    *s = SMB0DAT ;               //接收数据
        ACK = 1;                 	//发送应答位
        s++;
    } 
    SI = 0;
	while(!SI);
	*s = SMB0DAT;
    ACK = 0;                     //发送非应位
	STO = 1;                     //结束总线
    return(1);
}

/******************************************************************************
** 函 数 名: Bool SMBusSendStrExt(Uint8 sla,Uint8 *s,Uint8 no) 
** 功能描述: 向无子地址器件发送多字节数据函数   
**			 从启动总线到发送地址，子地址,数据，结束总线的全过程.
**			注意：    使用前必须已结束总线。
** 输　  入: 从器件地址sla，发送内容是s指向的内容，发送no个字节。
** 输　  出: 返回1表示操作成功，否则操作有误。
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
******************************************************************************/ 
Bool SMBusSendStrExt(Uint8 sla,Uint8 *s,Uint8 no) 
{
	Uint8 i;
   
	SI = 0;
	STO = 0;
	STA = 1;                    //启动总线
   	while(!SI);
	STA = 0;
   	SMB0DAT = sla;              //发送器件地址
	SI = 0;
	while(!SI);
	for(i=0;i<no;i++)
	{   
		SMB0DAT = (*s);            //发送数据
		SI = 0;
		while(!SI);
		s++;
	}
	ACK = 0;                     //发送非应位 
	STO = 1;               //结束总线 
	return(1);	 
}

