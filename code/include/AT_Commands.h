/*
**********************************************************************************
**�ļ�˵����AT_Commands.h                                                           
**�ļ����ƣ�ATָ�                                                    
**�������ڣ�2013.                                                         
**�� �� �ˣ�XZY                                                                   
**---------------------------------------------------------------------------------*
**�޸����ڣ�                                                                       
**�޸�˵����                                                                       
**********************************************************************************
*/
#ifndef AT_COMMANDS_H
#define AT_COMMANDS_H

#include "user_type.h"


extern code Uint8 ZBSNTOATMode[3];		//͸��ģʽ�л��� AT����ģʽ
extern code Uint8 ZBSNTOAPIMode[3];	    //͸��ģʽ�л��� API����ģʽ
extern code Uint8 ATSTA[3];				//AT ������ǰ׺
extern code Uint8 ATEND[2];			    //AT �����н�β
extern code Uint8 ATASK[1];				//
extern code Uint8 ATEQU[1];				//
extern code Uint8 ZBOK[4];
extern code Uint8 ZBERROR[7];
extern code Uint8 ZBESCATMode[3];		//AT����ģʽ�л���͸��ģʽ
extern code Uint8 ZBATNCD[3];			//��ѯ�ڵ㵱ǰʵ��Ӧ�õ������ַ
extern code Uint8 ZBATTID[3];			//���ýڵ�͸����ַ����
extern code Uint8 ZBATAST[3];			//���ýڵ��Զ�������������
extern code Uint8 ZBATPRF[3];		    //���ýڵ�����������������������
extern code Uint8 ZBATMOD[3];			//���ó�������Ӧ��ģʽ
extern code Uint8 ZBATSAV[3];			//������� 
extern code Uint8 ZBATSRS[3];			//ϵͳ����
extern code Uint8 ZBATAPI[3];
extern code Uint8 ZBATTYP[3];  	   		//���ýڵ�ڵ���������
extern code Uint8 ZBATNID[3];			//���ýڵ������ַ����
extern code Uint8 ZBESCAPI[6];			//�˳�API����
extern code Uint8 ZBAPIACK[6];			//API������ȷӦ��
extern code Uint8 ZBAPIDATOK[6];		//API���ݷ�����ȷӦ��

/*
-------------------------------------------------------------------------------------
˵��: GPRS Q2687 ATָ�PPP�������ݸ�ʽ
-------------------------------------------------------------------------------------
*/
extern code Uint8 GSMCommand0[3];					// test communication command
extern code Uint8 GSMCommand1[7];				// test csq ��������ź�ǿ�� 
extern code Uint8 GSMCommand2[12];			// connect gprs
extern code Uint8 GSMCommand3[5];					// echo on
extern code Uint8 GSMCommand4[8];              // ��ѯSIMID
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

//֡У���
extern code Uint16 fcstab[256];



#endif