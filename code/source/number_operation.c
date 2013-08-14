/***********************************************************************************/
/*�ļ�˵�������ֲ������                                                           */
/*�ļ����ƣ� number_operation.c                                                    */
/*�������ڣ�2012.10.11                                                             */
/*�� �� �ˣ�������                                                                   */
/*---------------------------------------------------------------------------------*/
/*�޸����ڣ�                                                                       */
/*�޸�˵����                                                                       */
/***********************************************************************************/
#include "memory.h"
//#include "user_cfg.h"

/*
*************************************************************************************
**�������ƣ�void NUM_HexToBCD(Uint8 dat,Uint8 offset)                             **
**�������ܣ���16�������ݲ��BCD��                                                  **
**��ڲ�����Uint8 dat ���������(С��100����)									   **
**          Uint8 offset �ڻ�����ƫ����                                            **
**���ز�������                                                                     **
*************************************************************************************
*/
void NUM_HexToBCD(Uint8 dat,Uint8 offset)
{
	BufDat->Dat[0 + offset] = (dat / 10) | 0x30;
	dat = dat % 10;
	BufDat->Dat[1 + offset] = dat | 0x30;
}
/*
*************************************************************************************
**�������ƣ�void NUM_CharHexToBCD(Uint8 dat,Uint8 offset)                          **
**�������ܣ���16�������ݲ��BCD��                                                  **
**��ڲ�����Uint8 dat ���������												   **
**          Uint8 offset �ڻ�����ƫ����                                            **
**���ز�������                                                                     **
*************************************************************************************
*/
void NUM_CharHexToBCD(Uint8 dat,Uint8 offset)
{
	BufDat->Dat[0 + offset] = (dat / 100) | 0x30;
	dat = dat % 100;
	BufDat->Dat[1 + offset] = (dat / 10) | 0x30;
	dat = dat % 10;
	BufDat->Dat[2 + offset] = dat | 0x30;
}

/*
*************************************************************************************
**�������ƣ�void NUM_ShortHexToBCD(Uint16 dat,Uint8 offset)                        **
**�������ܣ���16�������ݲ��BCD��                                                  **
**��ڲ�����Uint16 dat ���������												   **
**          Uint8 offset �ڻ�����ƫ����                                            **
**���ز�������                                                                     **
*************************************************************************************
*/
void NUM_ShortHexToBCD(Uint16 dat,Uint8 offset)
{
	BufDat->Dat[0 + offset] = (dat / 1000 ) | 0x30;
	dat = dat % 1000;
	BufDat->Dat[1 + offset] = (dat / 100 ) | 0x30;
	dat = dat % 100;
	BufDat->Dat[2 + offset] = (dat / 10 ) | 0x30;
	dat = dat % 10;
	BufDat->Dat[3 + offset] = dat | 0x30;
}
/*
*************************************************************************************
**�������ƣ�void NUM_ShortHexToBCD2(Uint16 dat,Uint8 offset)                       **
**�������ܣ���16�������ݲ��BCD��                                                  **
**��ڲ�����Uint16 dat ���������												   **
**          Uint8 offset �ڻ�����ƫ����                                            **
**���ز�������                                                                     **
*************************************************************************************
*/
void NUM_ShortHexToBCD2(Uint16 dat,Uint8 offset)
{
	BufDat->Dat[0 + offset] = (dat / 10000 ) | 0x30;
	dat = dat % 10000;
	BufDat->Dat[1 + offset] = (dat / 1000 ) | 0x30;
	dat = dat % 1000;
	BufDat->Dat[2 + offset] = (dat / 100 ) | 0x30;
	dat = dat % 100;
	BufDat->Dat[3 + offset] = (dat / 10 ) | 0x30;
	dat = dat % 10;
	BufDat->Dat[4 + offset] = dat | 0x30;
}
/*
*************************************************************************************
**�������ƣ�void NUM_LongHexToBCD(Uint16 dat,Uint8 offset)                         **
**�������ܣ���16�������ݲ��BCD��                                                  **
**��ڲ�����Uint32 dat ���������												   **
**          Uint8 offset �ڻ�����ƫ����                                            **
**���ز�������                                                                     **
*************************************************************************************
*/
/*void NUM_LongHexToBCD(Uint32 dat,Uint8 offset)
{
	BufDat->Dat[0 + offset] = (dat / 100000) | 0x30;
	dat = dat % 100000;
	BufDat->Dat[1 + offset] = (dat / 10000) | 0x30;
	dat = dat % 10000;
	BufDat->Dat[2 + offset] = (dat / 1000) | 0x30;
	dat = dat % 1000;
	BufDat->Dat[3 + offset] = (dat / 100) | 0x30;
	dat = dat % 100;
	BufDat->Dat[4 + offset] = (dat / 10) | 0x30;
	dat = dat % 10;
	BufDat->Dat[5 + offset] = dat | 0x30; 
} */

/*
*************************************************************************************
**�������ƣ�Uint32 NUM_GetBackDis(Uint8 n,Uint8 offset)                            **
**�������ܣ��ӻ����лض���ֵ                                                       **
**��ڲ�����Uint8 n �ض�����													   **
**          Uint8 offset ����ƫ����                                                **
**���ز�������                                                                     **
*************************************************************************************
*/
/*Uint32 NUM_GetBackDis(Uint8 n,Uint8 offset)
{
	Uint32 num;
	Uint8 i;
	for(i = 0,num = 0;i < n;i ++)
	{
		num = num * 10 + (BufDat->Dat[i + offset] & 0x0f);
	} 
	return num;
} */

