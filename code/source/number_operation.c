/***********************************************************************************/
/*文件说明：数字操作相关                                                           */
/*文件名称： number_operation.c                                                    */
/*创建日期：2012.10.11                                                             */
/*创 建 人：熊招育                                                                   */
/*---------------------------------------------------------------------------------*/
/*修改日期：                                                                       */
/*修改说明：                                                                       */
/***********************************************************************************/
#include "memory.h"
//#include "user_cfg.h"

/*
*************************************************************************************
**函数名称：void NUM_HexToBCD(Uint8 dat,Uint8 offset)                             **
**函数功能：将16进制数据拆分BCD码                                                  **
**入口参数：Uint8 dat 待拆分数据(小于100的数)									   **
**          Uint8 offset 在缓存中偏移量                                            **
**返回参数：无                                                                     **
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
**函数名称：void NUM_CharHexToBCD(Uint8 dat,Uint8 offset)                          **
**函数功能：将16进制数据拆分BCD码                                                  **
**入口参数：Uint8 dat 待拆分数据												   **
**          Uint8 offset 在缓存中偏移量                                            **
**返回参数：无                                                                     **
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
**函数名称：void NUM_ShortHexToBCD(Uint16 dat,Uint8 offset)                        **
**函数功能：将16进制数据拆分BCD码                                                  **
**入口参数：Uint16 dat 待拆分数据												   **
**          Uint8 offset 在缓存中偏移量                                            **
**返回参数：无                                                                     **
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
**函数名称：void NUM_ShortHexToBCD2(Uint16 dat,Uint8 offset)                       **
**函数功能：将16进制数据拆分BCD码                                                  **
**入口参数：Uint16 dat 待拆分数据												   **
**          Uint8 offset 在缓存中偏移量                                            **
**返回参数：无                                                                     **
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
**函数名称：void NUM_LongHexToBCD(Uint16 dat,Uint8 offset)                         **
**函数功能：将16进制数据拆分BCD码                                                  **
**入口参数：Uint32 dat 待拆分数据												   **
**          Uint8 offset 在缓存中偏移量                                            **
**返回参数：无                                                                     **
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
**函数名称：Uint32 NUM_GetBackDis(Uint8 n,Uint8 offset)                            **
**函数功能：从缓存中回读数值                                                       **
**入口参数：Uint8 n 回读个数													   **
**          Uint8 offset 缓存偏移量                                                **
**返回参数：无                                                                     **
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

