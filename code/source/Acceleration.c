/*
**********************************************************************************
** 文件说明：MMA8452加速度芯片操作                                                       
** 文件名称：Acceleration.c                                                 
** 创建日期：2013.05                                                         
** 创 建 人：XZY                                                                  
**---------------------------------------------------------------------------------*
** 修改日期：                                                                    
** 修改说明：                                                                    
**********************************************************************************
*/
/*
-------------------------------------------------------------------------------------
说明:include 
-------------------------------------------------------------------------------------
*/
#include "Interrupt.h"
#include "Acceleration.h"
#include "iic.h"
#include "memory.h"
/*
*****************************************************************************
** 函 数 名: Bool MMA8452_RegWrite(Uint8 Addr,Uint8 Dat)
** 功能描述: 向MMA8452寄存器写数据
** 输　  入: Addr寄存器地址，Dat数据
** 输　  出: TRUER/FALSE  
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/
Bool MMA8452_RegWrite(Uint8 Addr,Uint8 Dat)
{
	Uint8 i;
	i = 0;
	while(!(ISendStr(ADDR_MMA8452Q,(Uint16)Addr,(Uint8 *)&Dat,1,0)))
	{
		i ++;
		if(i > 3)
		{
			return FALSE;
		}
	}
	return TRUER;
} 
/*
*****************************************************************************
** 函 数 名: Bool MMA8452_Read(Uint8 Addr,Uint8 * Buf,Uint8 Count)
** 功能描述: 从MMA8452读数据
** 输　  入: Addr Buf Count
** 输　  出: TRUER/FALSE    
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/
Bool MMA8452_Read(Uint8 Addr,Uint8 * Buf,Uint8 Count)
{
	Uint8 i;
	i = 0;
	if(!(IRcvStr(ADDR_MMA8452Q,(Uint16)Addr,Buf,Count,0)))
	{
		i ++;
		if(i > 3)
		{
			return FALSE;
		}
	}
	return TRUER;
}
/*
*****************************************************************************
** 函 数 名: Uint8 MMA8452_RegRead(Uint8 Addr)
** 功能描述: 读MMA8452寄存器
** 输　  入: Addr
** 输　  出: 数据
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/
Uint8 MMA8452_RegRead(Uint8 Addr)
{
	Uint8 i;
	MMA8452_Read(Addr,(Uint8 *)&i,1);
	return i;
} 
/*
*****************************************************************************
** 函 数 名: Bool MMA8452_Standby(void)
** 功能描述: MMA8452进待机模式
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/ 
Bool MMA8452_Standby(void)
{
	Uint8 i;
	i = MMA8452_RegRead(CTRL_REG1);
	if(MMA8452_RegWrite(CTRL_REG1,i & ~ACTIVE_MASK) == TRUER)
	{
		return TRUER;
	}
	return FALSE;
}
/*
*****************************************************************************
** 函 数 名: void MMA8452_Active(void)
** 功能描述: MMA8452进正常工作模式
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/ 
void MMA8452_Active(void)
{
	//Uint8 i;
	MMA8452_RegWrite(CTRL_REG1,(MMA8452_RegRead(CTRL_REG1)|ACTIVE_MASK));	
	//i = MMA8452_RegRead(CTRL_REG1);
}
/*
*****************************************************************************
** 函 数 名: Bool MMA8452_Init(void)
** 功能描述: MMA8452初始化
				2G模式,数据输出速率(200Hz/5ms),中断信号连接到INT1引脚（默认推挽输出低电平有效）
** 输　  入: 无
** 输　  出: TRUER/FALSE 
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/ 
Bool MMA8452_Init(void)
{
	if(MMA8452_Standby() == TRUER)		//进待机模式
	{
		MMA8452_RegWrite(XYZ_DATA_CFG,(MMA8452_RegRead(XYZ_DATA_CFG) & ~FS_MASK));//2G模式
	//	MMA8452_RegWrite(XYZ_DATA_CFG,(MMA8452_RegRead(XYZ_DATA_CFG) | FULL_SCALE_4G));//4G,8G模式需加此句
		MMA8452_RegWrite(CTRL_REG1,(MMA8452_RegRead(CTRL_REG1) & ~DR_MASK));		//清数据输出速率
		MMA8452_RegWrite(CTRL_REG1,(MMA8452_RegRead(CTRL_REG1)|(DataRateValue << 3)));//设置数据输出速率
		MMA8452_RegWrite(CTRL_REG4,INT_EN_DRDY);	//数据就绪中断使能
		MMA8452_RegWrite(CTRL_REG5,INT_CFG_DRDY);	//中断信号连接到INT1引脚
		MMA8452_Active();		//正常工作模式
		MMA8452_Standby();		//进待机模式
		return TRUER;
	}
	return FALSE;
}/*
*****************************************************************************
** 函 数 名: void Port_Match_Init(void)
** 功能描述: 端口匹配中断初始化
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/ 
void Port_Match_Init(void)
{
	//P1MAT = P1MAT &(~ACCEOK);	//P1.2引脚逻辑值与逻辑低电平比较。
	P1MAT = P1MAT | ACCEOK;	//P1.2引脚逻辑值与逻辑高电平比较。
	P1MASK = P1MASK | ACCEOK;	//允许P1.2引脚逻辑进行比较
	MATIntSwitchOn();			//允许端口匹配中断	
}
/*
*****************************************************************************
** 函 数 名: void ACCEL_Init(void)
** 功能描述: 加速度初始化
** 输　  入: 无
** 输　  出: TRUER/FALSE 
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/ 
void ACCEL_Init(void)
{
	 MATIntSwitchOff();	//端口匹配中断关闭
	 G_POWERON();		  //开MMA8452电源	 
	 if(MMA8452_Init() == FALSE)
	 {
		 ClearMask(SysDat.Flag,ACC_INT_MASK);
	 }		 
	 else
	 {
		Port_Match_Init();	  //端口匹配中断配置、使能
		SetMask(SysDat.Flag,ACC_INT_MASK);
	 }
	 
}
/*
*****************************************************************************
** 函 数 名: void ACCEL_OFF(void)
** 功能描述: 关闭加速度模块
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/
void ACCEL_OFF(void)
{
	MATIntSwitchOff();	//端口匹配中断关闭
	G_POWEROFF();		//关MMA8452电源
	ClearMask(SysDat.Flag,ACC_INT_MASK);
} 
/*
*****************************************************************************
** 函 数 名: void MATCH_INT(void) interrupt PORT_MATCH_PRIOR_ORDER
** 功能描述: 端口匹配中断,用于自动响应MMA8452数据就绪中断
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/
void MATCH_INT(void) interrupt PORT_MATCH_PRIOR_ORDER
{
	Uint8 flag;
	Uint8 buf[6];
	GlobleIntSwitchOff();		//全局中断关
	flag = MMA8452_RegRead(STATUS_00_REG);
	if(flag & (INT_EN_DRDY<<2))  //Z轴数据OK
	{
		MMA8452_Read(OUT_X_MSB,buf,6);
		MMA8452_Standby();
		GatDat->TempAcc[SysDat.pFilter] = buf[4] * 256 + buf[5];
		GatDat->TempAcc[SysDat.pFilter] = GatDat->TempAcc[SysDat.pFilter] >> 4; 		
		SetMask(SysDat.Flag,ACC_DATA_MASK);
	}
	GlobleIntSwitchOn();		//全局中断开
} 
