/***********************************************************************************/
/*文件说明：数据类型定义文件                                                       */
/*文件名称：user_type.h                                                            */
/*创建日期：2013.04                                                         	 */
/*创 建 人：xzy                                                                  */
/*---------------------------------------------------------------------------------*/
/*修改日期：                                                                       */
/*修改说明：                                                                       */
/***********************************************************************************/
#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "user_cfg.h"




//---------------------------------------------------------------------------------
//说明：数据类型，位长度。
//---------------------------------------------------------------------------------
typedef bit           Bool;
typedef bit           Bit;
typedef unsigned char Uint8;
typedef unsigned int  Uint16;
typedef unsigned long Uint32;
typedef signed char   Int8;
typedef signed int    Int16;
typedef signed long   Int32;
typedef float         Float32;

#define		BYTE		unsigned char
#define 	WORD 		unsigned int
#define 	UCHAR 		unsigned char
#define 	UINT  		unsigned int

//---------------------------------------------------------------------------------
//说明：系统状态数据结构   15
//---------------------------------------------------------------------------------
typedef struct
{
	Uint8 Flag;			//标志位（含义见user_cfg.h掩码解释）
	Uint8 flagGather;   //数据采集标志
	Uint8 timeLeft;	   //中断次数计数器
	Uint8 pFilter;
	Uint8 pADC;
	Uint8 channelADC;
	Uint16 MainLoopError;//复位计时器
	Uint16 SecCount;	//秒计数器
	Uint16 PointAddr;   //数据存储地址
	Uint16 Index;		//数据索引
	Uint16 BuffAddr;	//缓存地址
	Uint8 Mode;
	Uint8 NumRegather;  //数据采集次数，不能超过5
	Uint8 ResendTimer;   //数据从发时间
	Uint8 TimerTest;     //
	//Uint8 Link_Timer;   //GPRS连接时间
	Uint8 TimeResend;	//数据从发次数
}SYS_MODEDATA;
//---------------------------------------------------------------------------------
//说明：系统参数  17，只存前12字节
//---------------------------------------------------------------------------------
typedef struct
{
	Uint16 SpaceTime;	//发送数据间隔,对应MESSAGE_DATA->spaceTime    分钟
	Uint16 DeviceNo;    //编号, 对应MESSAGE_DATA->deviceNo	
	Uint16 Port;              //服务器中心端口号
	Uint8 DeviceIpAddr[4];   //服务器中心IP
	Uint8 TimeUnit;		//采样间隔，对应MESSAGE_DATA->timeAD
	Uint8 MemInitFlag;	//高4位用作存储器初始化标志 A0 ，低4位用作记录编号修改次数
	Uint8 RecCount;     //接收数据计数（不能大于MAX_POINT_NUM）	
	Uint8 voltage;
	Uint8 temperature;
	Uint8 channelADC;
	Uint8 csqGPRS;     //GPRS信号强度
}PARAMETER;
typedef PARAMETER * PARAMETERP;
//---------------------------------------------------------------------------------
//说明：缓存数据 304
//---------------------------------------------------------------------------------
typedef struct
{
	Uint8 Dat[BUF_NUM];
	Uint16 BitPoint;
	Uint16 NumCount;
	//Uint8 * Point; 
}BUF_DAT;
typedef BUF_DAT * BUF_DATP;
//-----------------------------------------------------------------------------------
//说明：数据采集用缓存   1480
//-----------------------------------------------------------------------------------
typedef struct
{
	Uint8 LoadBuff[TOTAL];	    //240
	Uint8 AccBuff[TOTAL];       //240
	Uint8 Num[20];
	Uint16 TempLoad[5];
	Uint16 TempAcc[5];
	Uint16 AccInt[TOTAL];       //480
	Uint16 Area[TOTAL];         //480	
}GATHER_DAT;
typedef GATHER_DAT * GATHER_DATP;
//-----------------------------------------------------------------------------------
//说明：GPRS连接缓存   522+400+350 = 1272
//-----------------------------------------------------------------------------------
typedef struct
{
	Uint8 RECEIVEBUFFER[100];
	Uint8 WAVE_Command_Buf[360];
	Uint8 UDP_Buffer[350];           //CHECKSUM_UDP函数中的Temp_Buffer[350]
	Uint8 SEND_Buffer[400];          //SEND_DATA_WAVECOM函数中的Temp_Buffer[400]		
	Uint8 IP_Head[20];
	Uint8 UDP_Head[8];
	Uint8 EXCHANGE[30];	
	Uint8 *RECEIVEBUFFERPOINT;
	Uint8 *UDP_Data;
	Uint8 WAVE_Receive_Num;
	Uint8 UDP_Data_Len;	
}LINK_DAT;
typedef LINK_DAT * LINK_DATP;
/*typedef struct	
{
		BYTE   ipAddr[4];
		WORD   port;
}IP_PORT;	 
typedef IP_PORT * IP_PORTP;  */
//---------------------------------------------------------------------------------------------
//说明：信息发送结构 14+120+120+20+1 = 275
//---------------------------------------------------------------------------------------------
typedef struct 
{
	Uint8 Head[2];                    //** 0x2a,0x2a       
	Uint16 length;                   //3
	Uint16 deviceNo;                 //5
	Uint8 seq;        //报文序列号   //7
	Uint8 csq;        //信号强度     //8
	Uint8 mode;		 //9             
	Uint8 zxh;		 //10
	Uint8 voltage;   //11
	Uint8 temperature; //12
	Uint8 spaceTime;        //发送间隔
	Uint8 timeAD;           //采集时间
	Uint8 Load[POINTS_NUM];
	Uint8 Weiy[POINTS_NUM];				// this 2.3version位移  120
	Uint8 SIMID[20];                     //AT+CCID 查询  +CCID: "89860057300645485868" OK
	Uint8 checkSum;
}MESSAGE_DATA;
typedef MESSAGE_DATA * MESSAGE_DATAP;
	
#endif