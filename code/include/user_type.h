/***********************************************************************************/
/*�ļ�˵�����������Ͷ����ļ�                                                       */
/*�ļ����ƣ�user_type.h                                                            */
/*�������ڣ�2013.04                                                         	 */
/*�� �� �ˣ�xzy                                                                  */
/*---------------------------------------------------------------------------------*/
/*�޸����ڣ�                                                                       */
/*�޸�˵����                                                                       */
/***********************************************************************************/
#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "user_cfg.h"




//---------------------------------------------------------------------------------
//˵�����������ͣ�λ���ȡ�
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
//˵����ϵͳ״̬���ݽṹ   15
//---------------------------------------------------------------------------------
typedef struct
{
	Uint8 Flag;			//��־λ�������user_cfg.h������ͣ�
	Uint8 flagGather;   //���ݲɼ���־
	Uint8 timeLeft;	   //�жϴ���������
	Uint8 pFilter;
	Uint8 pADC;
	Uint8 channelADC;
	Uint16 MainLoopError;//��λ��ʱ��
	Uint16 SecCount;	//�������
	Uint16 PointAddr;   //���ݴ洢��ַ
	Uint16 Index;		//��������
	Uint16 BuffAddr;	//�����ַ
	Uint8 Mode;
	Uint8 NumRegather;  //���ݲɼ����������ܳ���5
	Uint8 ResendTimer;   //���ݴӷ�ʱ��
	Uint8 TimerTest;     //
	//Uint8 Link_Timer;   //GPRS����ʱ��
	Uint8 TimeResend;	//���ݴӷ�����
}SYS_MODEDATA;
//---------------------------------------------------------------------------------
//˵����ϵͳ����  17��ֻ��ǰ12�ֽ�
//---------------------------------------------------------------------------------
typedef struct
{
	Uint16 SpaceTime;	//�������ݼ��,��ӦMESSAGE_DATA->spaceTime    ����
	Uint16 DeviceNo;    //���, ��ӦMESSAGE_DATA->deviceNo	
	Uint16 Port;              //���������Ķ˿ں�
	Uint8 DeviceIpAddr[4];   //����������IP
	Uint8 TimeUnit;		//�����������ӦMESSAGE_DATA->timeAD
	Uint8 MemInitFlag;	//��4λ�����洢����ʼ����־ A0 ����4λ������¼����޸Ĵ���
	Uint8 RecCount;     //�������ݼ��������ܴ���MAX_POINT_NUM��	
	Uint8 voltage;
	Uint8 temperature;
	Uint8 channelADC;
	Uint8 csqGPRS;     //GPRS�ź�ǿ��
}PARAMETER;
typedef PARAMETER * PARAMETERP;
//---------------------------------------------------------------------------------
//˵������������ 304
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
//˵�������ݲɼ��û���   1480
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
//˵����GPRS���ӻ���   522+400+350 = 1272
//-----------------------------------------------------------------------------------
typedef struct
{
	Uint8 RECEIVEBUFFER[100];
	Uint8 WAVE_Command_Buf[360];
	Uint8 UDP_Buffer[350];           //CHECKSUM_UDP�����е�Temp_Buffer[350]
	Uint8 SEND_Buffer[400];          //SEND_DATA_WAVECOM�����е�Temp_Buffer[400]		
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
//˵������Ϣ���ͽṹ 14+120+120+20+1 = 275
//---------------------------------------------------------------------------------------------
typedef struct 
{
	Uint8 Head[2];                    //** 0x2a,0x2a       
	Uint16 length;                   //3
	Uint16 deviceNo;                 //5
	Uint8 seq;        //�������к�   //7
	Uint8 csq;        //�ź�ǿ��     //8
	Uint8 mode;		 //9             
	Uint8 zxh;		 //10
	Uint8 voltage;   //11
	Uint8 temperature; //12
	Uint8 spaceTime;        //���ͼ��
	Uint8 timeAD;           //�ɼ�ʱ��
	Uint8 Load[POINTS_NUM];
	Uint8 Weiy[POINTS_NUM];				// this 2.3versionλ��  120
	Uint8 SIMID[20];                     //AT+CCID ��ѯ  +CCID: "89860057300645485868" OK
	Uint8 checkSum;
}MESSAGE_DATA;
typedef MESSAGE_DATA * MESSAGE_DATAP;
	
#endif