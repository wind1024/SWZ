/*
**********************************************************************************
**文件说明：AT_Commands.h                                                           
**文件名称：AT指令集                                                    
**创建日期：2013.                                                         
**创 建 人：XZY                                                                   
**---------------------------------------------------------------------------------*
**修改日期：                                                                       
**修改说明：                                                                       
**********************************************************************************
*/
#ifndef AT_COMMANDS_H
#define AT_COMMANDS_H

#include "user_type.h"


extern code Uint8 ZBSNTOATMode[3];		//透传模式切换到 AT命令模式
extern code Uint8 ZBSNTOAPIMode[3];	    //透传模式切换到 API命令模式
extern code Uint8 ATSTA[3];				//AT 命令行前缀
extern code Uint8 ATEND[2];			    //AT 命令行结尾
extern code Uint8 ATASK[1];				//
extern code Uint8 ATEQU[1];				//
extern code Uint8 ZBOK[4];
extern code Uint8 ZBERROR[7];
extern code Uint8 ZBESCATMode[3];		//AT命令模式切换到透传模式
extern code Uint8 ZBATNCD[3];			//查询节点当前实际应用的网络地址
extern code Uint8 ZBATTID[3];			//设置节点透传地址命令
extern code Uint8 ZBATAST[3];			//设置节点自动加入网络命令
extern code Uint8 ZBATPRF[3];		    //设置节点接收无线数据输出串口命令
extern code Uint8 ZBATMOD[3];			//设置程序运行应用模式
extern code Uint8 ZBATSAV[3];			//保存参数 
extern code Uint8 ZBATSRS[3];			//系统重启
extern code Uint8 ZBATAPI[3];
extern code Uint8 ZBATTYP[3];  	   		//设置节点节点类型命令
extern code Uint8 ZBATNID[3];			//设置节点网络地址命令
extern code Uint8 ZBESCAPI[6];			//退出API命令
extern code Uint8 ZBAPIACK[6];			//API命令正确应答
extern code Uint8 ZBAPIDATOK[6];		//API数据发送正确应答

/*
-------------------------------------------------------------------------------------
说明: GPRS Q2687 AT指令及PPP连接数据格式
-------------------------------------------------------------------------------------
*/
extern code Uint8 GSMCommand0[3];					// test communication command
extern code Uint8 GSMCommand1[7];				// test csq 检查网络信号强度 
extern code Uint8 GSMCommand2[12];			// connect gprs
extern code Uint8 GSMCommand3[5];					// echo on
extern code Uint8 GSMCommand4[8];              // 查询SIMID
//extern code Uint8 BaudrateInit[];			// baud rate is 9600
extern code Uint8 EXCHANGE1_CODE[22];
extern code Uint8 EXCHANGE2_CODE[30];
extern code Uint8 EXCHANGE3_CODE[21];
extern code Uint8 EXCHANGE4_CODE[21];
extern code Uint8 EXCHANGE5_CODE[16];
extern code Uint8 EXCHANGE6_CODE[16];
extern code Uint8 EXCHANGE7_CODE[16];
extern code Uint8 EXCHANCE8[36];  // disconnect request
extern code Uint8 IP_Head_CODE[20];
extern code Uint8 UDP_Head_CODE[8];

//帧校验表
extern code Uint16 fcstab[256];



#endif