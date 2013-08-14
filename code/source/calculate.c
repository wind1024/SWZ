/*
**********************************************************************************
**文件说明：运算相关                                                            
**文件名称：calculate.c                                                   
**创建日期：2013.                                                 
**创 建 人：XZY                                                                 
**---------------------------------------------------------------------------------*
**修改日期：2013.07.16                                                                    
**修改说明：对以前程序算法进行整理分类后形成此文件，对内存占用过大进行优化，但并未改变其实质                                                                    
**********************************************************************************
*/
#include "user_type.h"
#include <math.h>
#include "memory.h"
#include "diskio.h"

//----------------------------------------------------------------------------
//说明：基本数据处理
//----------------------------------------------------------------------------
/*
*****************************************************************************
**函 数 名：Uint16 Filter(Uint16 *p,Uint8 n)
**功能描述：求数组数据的中间值(中值滤波)
**输    入：数据数组首地址Uint16 *p，数组长度Uint8 n 
**输    出：数组数据中间值
*****************************************************************************
*/
Uint16 Filter(Uint16 *p,Uint8 n)	//	求数组数据的中间值
{
	Uint8 xdata i,j;
	Uint16 xdata tempData;
	Uint32 xdata temp;
	
	if(n<=1)
		return *p;
	else
	{
		for(j=0;j<n-1;j++)									// 从小到大排序
		{
			for(i=0;i<(n-j-1);i++)
			{
				if(*(p+i)>*(p+i+1))
				{
					tempData = *(p+i);
					*(p+i) = *(p+i+1);
					*(p+i+1) = tempData;
				}
			}
		}
		if(n%2)
			return *(p+(n-1)/2);
		else
		{
			temp = *(p+n/2)+*(p+n/2-1);
			if(temp%2)	temp++;								// rather bigger 1
			return (Uint16)(temp/2);
		}
			
	}
}
/*
*****************************************************************************
**函 数 名：Uint8 MsgCheckSum(Uint8 *addr,Uint16 length)
**功能描述：异或校验和
**输    入：Uint8 *addr,Uint16 length 
**输    出：8位异或校验和
*****************************************************************************
*/
Uint8 MsgCheckSum(Uint8 *addr,Uint16 length)//异或校验
{
	Uint8 xdata sum = 0;
	for(;length>0;length--)
		sum ^= *((Uint8*)addr++);
	return sum;
}
/*
*****************************************************************************
**函 数 名：Uint8 MinCalculate(Uint8 *p,Uint16 n)
**功能描述：找最小值
**输    入：Uint8 *p,Uint16 n 
**输    出：最小值
*****************************************************************************
*/
Uint8 MinCalculate(Uint8 *p,Uint8 n)//找最小值
{
	Uint8 xdata sum = 0xff;
	Uint8 xdata i;
	for(i=0;i<n;i++)
	{
		if(*(p+i)<sum)
			sum = *(p+i);
	}
	return sum;
}
/*
*****************************************************************************
**函 数 名：Uint8 MaxCalculate(Uint8 *p,Uint8 n)
**功能描述：找最大值
**输    入：Uint8 *p,Uint8 n 
**输    出：最大值
*****************************************************************************
*/
Uint8 MaxCalculate(Uint8 *p,Uint8 n)//找最大值
{
	Uint8 xdata sum = 0;
	Uint8 xdata i;
	for(i=0;i<n;i++)
	{
		if(*(p+i)>sum)
			sum = *(p+i);
	}
	return sum;
}
/*
*****************************************************************************
**函 数 名：Float32 Average(Uint8 *p,Uint8 n)
**功能描述：求均值
**输    入：Uint8 *p,Uint8 n 
**输    出：平均值
*****************************************************************************
*/
Float32 Average(Uint8 *p,Uint8 n)//求均值
{
	Float32 xdata sum = 0;
	Uint8 xdata i;
	for(i=0;i<n;i++)
		sum += *(p+i);
	return (sum/n);
}
/*
*****************************************************************************
**函 数 名：Uint8 SerialLess(Uint8 *p,Uint8 n)
**功能描述：统计数组中小于平均值的数连续出现的最长长度
**输    入：Uint8 *p,Uint8 n
**输    出：结果
*****************************************************************************
*/
Uint8 SerialLess(Uint8 *p,Uint8 n)         //统计数组中小于平均值的数连续出现的最长长度
{
	Uint8 xdata i,belowCnt,tempBelowCnt,aver;

	belowCnt=tempBelowCnt=0;
	aver = Average(p,n);
	for(i=0;i<n;i++)
	{
		if(*(p+i)<aver)
			tempBelowCnt ++;                             //小于平均值的数统计
		else
		{
			if(tempBelowCnt>belowCnt)
				belowCnt=tempBelowCnt;
			tempBelowCnt=0;
		}
	}
	if(tempBelowCnt>belowCnt)
		belowCnt=tempBelowCnt;
	return belowCnt;
}
/*
*****************************************************************************
**函 数 名：Uint8 SerialMore(Uint8 *p,Uint8 n)
**功能描述：统计数组中大于平均值的数连续出现的最长长度
**输    入：Uint8 *p,Uint8 n 
**输    出：结果
*****************************************************************************
*/
Uint8 SerialMore(Uint8 *p,Uint8 n)      //统计数组中大于平均值的数连续出现的最长长度
{
	Uint8 xdata i,overCnt,tempOverCnt,aver;

	overCnt=tempOverCnt=0;
	aver = Average(p,n);
	for(i=0;i<n;i++)
	{
		if(*(p+i)>aver)
			tempOverCnt ++;                  //大于平均值的数统计
		else
		{
			if(tempOverCnt>overCnt)
				overCnt=tempOverCnt;
			tempOverCnt=0;
		}
	}
	if(tempOverCnt>overCnt)
		overCnt=tempOverCnt;
	return overCnt;
}
/*
*****************************************************************************
**函 数 名：void CalculateArea(Uint8 *p,Uint8 n)
**功能描述：计算面积
**输    入：Uint8 *p,Uint8 n 
**输    出：
*****************************************************************************
*/
void CalculateArea(Uint8 *p,Uint8 n)    //计算面积
{
	Uint8 xdata i,j;
	Uint16 xdata temp,sum;	
	Uint8 start = 0;

	for(i=0;i<(TOTAL-n);i++)
	{
		sum = 0;
		for(j=0;j<=n;j++)
		{
			temp = abs(*(p+j+start)-*(p+i+j));//绝对值
			sum += temp;
		}
		GatDat->Area[i]=sum;
	}
}
/*
*****************************************************************************
**函 数 名：Uint8 FindLeastValue(Uint16 *p,Uint8 n)
**功能描述：按要求找到20个满足条件的数的序号
**输    入：Uint16 *p,Uint8 n 
**输    出：
*****************************************************************************
*/
Uint8 FindLeastValue(Uint16 *p,Uint8 n)     //按要求找到20个满足条件的数的序号
{
	Uint8 xdata i,j,temp,account = 0;
	Uint8 xdata tempNum[80],tempAcc = 0;
	// get 80 minimum value	
	for(i=0;i<n;i++)
	{
		if(i==0)
		{
			if((p[i]<=p[i+1]&&p[i+1]<p[i+2])||(p[i]<p[i+1]&&p[i+1]<=p[i+2]))
				tempNum[tempAcc++]=i;
			continue;
		}
		if(i==(n-1))
		{
			if((p[i-2]>=p[i-1]&&p[i-1]>p[i])||(p[i-2]>p[i-1]&&p[i-1]>=p[i]))
				tempNum[tempAcc++]=i;
			continue;
		}
		if((p[i]<=p[i+1])&&(p[i]<p[i-1]))
		{
			j = 0;
			if(p[i]==p[i+1])
			{
				for(j=0;j<(n-1-i);j++)
				{
					if(p[i+j]>p[i+j+1])
					{
						j= 0xff;
						break;
					}
					if(p[i+j]<p[i+j+1])
					{
						j = 0;
						break;
					}
				}
			}
			if(!j)
			{
				tempNum[tempAcc++]=i;
				if(tempAcc>=80)
					break;
			}
		}
	}	
	// line tempNum[]
	for(i=1;i<tempAcc;i++)                                 //从小到大排序
	{
		for(j=0;j<(tempAcc-i);j++)
		{
			if(*(p+tempNum[j])>*(p+tempNum[j+1]))
			{
				temp = tempNum[j];
				tempNum[j]=tempNum[j+1];
				tempNum[j+1]=temp;
			}
		}
	}
	// get 20 least value
	for(account=0;account<tempAcc;account++)
	{
		GatDat->Num[account]=tempNum[account];
		if(account>=20)
			break;
	}
	// line num[]
	for(i=1;i<account;i++)
	{
		for(j=0;j<(account-i);j++)
		{
			if(GatDat->Num[j] > GatDat->Num[j+1])
			{
				temp = GatDat->Num[j];
				GatDat->Num[j] = GatDat->Num[j+1];
				GatDat->Num[j+1] =temp;
			}
		}
	}
	return account;
}
/*
*****************************************************************************
**函 数 名：void DeleteArray(Uint8 len,Uint8 pos)
**功能描述：删除第 pos个数，后面数据前移一个位置
**输    入：Uint8 len,Uint8 pos 
**输    出：
*****************************************************************************
*/
void DeleteArray(Uint8 len,Uint8 pos)          //删除第 pos个数，后面数据前移一个位置
{
	Uint8 xdata i;
	for(i=0;i<(len-pos-1);i++)
	{
		GatDat->Num[pos+i] = GatDat->Num[pos+i+1];
	}
}
/*
*****************************************************************************
**函 数 名：Uint8 DistanceEqu(Uint8 len)
**功能描述：
**输    入：Uint8 len 
**输    出：
*****************************************************************************
*/
Uint8 DistanceEqu(Uint8 len)
{
	Uint8 xdata i,distance;
	if(len<3)
		return 0;
	for(i=0;i<(len-2);i++)
	{
		if(abs(GatDat->Num[i]-2*GatDat->Num[i+1]+GatDat->Num[i+2]) < ZQWC)
		{
			distance = abs(GatDat->Num[i+1]-GatDat->Num[i]);
			return distance;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
// 说明：算法
//---------------------------------------------------------------------------
/*
*****************************************************************************
**函 数 名：Uint8 JudgeWellStatus(Uint8 *p,Uint8 n)
**功能描述：载荷数据判断
**输    入：Uint8 *p,Uint8 n
**输    出：
*****************************************************************************
*/
Uint8 JudgeWellStatus(Uint8 *p,Uint8 n)          //载荷数据判断
{
	Float32 xdata average;
	Uint8 xdata max,min;
	average = Average(p,n);
	max = MaxCalculate(p,n);
	min = MinCalculate(p,n);

	if(average<36)
	{
		return 3;
	}
	else if(average<50 || max<47)
	{
		return 2;
	}
	else if((SerialLess(p,n)<MIN_DIF||SerialMore(p,n)<MIN_DIF)||(max-min)<MIN_DIF)     //范围不超MIN_DIF：5
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*
*****************************************************************************
**函 数 名：Uint8 GetCycle(Uint8 *p,Uint8 n)
**功能描述：算周期
**输    入：Uint8 *p,Uint8 n
**输    出：
*****************************************************************************
*/
Uint8 GetCycle(Uint8 *p,Uint8 n)                //算周期
{
	Uint8 xdata i,j,account,condition,tempMin,posMin,posMax;
	Uint16 xdata tempMax;
	Uint8 xdata cycle = 0;

	condition =(Uint16)(SerialMore(p,n)+SerialLess(p,n))*4/5;
	for(i=MIN_POINT;i<=MAX_POINT;i++)
	{
		CalculateArea(p,i);
		account=FindLeastValue(GatDat->Area,TOTAL-i);
		while(account>=3)
		{
			tempMin = 0xff;
			posMin = 0;
			for(j=0;j<(account-1);j++)
			{
				if(abs(GatDat->Num[j+1]-GatDat->Num[j])<tempMin)
				{
					tempMin = abs(GatDat->Num[j+1]-GatDat->Num[j]);
					posMin  = j;
				}
			}
			if(tempMin<condition)
			{
				if(GatDat->Area[GatDat->Num[posMin]] > GatDat->Area[GatDat->Num[posMin+1]])
					DeleteArray(account,posMin);
				else
					DeleteArray(account,posMin+1);
				account --;
			}
			else
			{
				cycle=DistanceEqu(account);
				if(cycle)
					return cycle;
				else
				{
					tempMax = 0;
					posMax = 0xff;
					for(j=0;j<account;j++)
					{
						if(GatDat->Area[GatDat->Num[j]]>=tempMax)
						{
							tempMax = GatDat->Area[GatDat->Num[j]];
							posMax  = j;
						}
					}
					DeleteArray(account,posMax);
					account --;
				}
			}
		}
	}
	return 0;
}
/*
*****************************************************************************
**函 数 名：Bool GetDisplacement(Uint16 *p,Uint8 cycle)
**功能描述：
**输    入：Uint16 *p,Uint8 cycle 
**输    出：
*****************************************************************************
*/
Bool GetDisplacement(Uint16 *p,Uint8 cycle)
{
	Uint8 xdata i,j,k;
	Uint8 xdata NO = TOTAL;	  //240
	Uint8 xdata qnm = QNI*2;  //14
	//Float32 xdata qna[TOTAL][QNI*2];
	Float32 xdata AccFloat[TOTAL],qnnn[QNI*2][QNI*2+1],qnx[QNI*2];      //240*4+14*15*4+14*4  =  1856
	Float32 xdata qnw,max,min,stroke;                                 //4*4
	SysDat.BuffAddr = DISK_BUFF_ADDR;
	qnw = 2*3.1415926/cycle;
	for(i=0;i<NO;i++)
	{
		qnx[0] 	= 1;
		for(j=1;j<qnm;j++)
		{
			if(j%2)		qnx[j] = sin(qnw*i*(Uint8)((j+1)/2));
			else		qnx[j] = cos(qnw*i*(Uint8)(j/2));
		}
		DISK_Write(SysDat.BuffAddr,(Uint8 *)qnx,sizeof(qnx));
		SysDat.BuffAddr = SysDat.BuffAddr + sizeof(qnx);
		AccFloat[i]	= (float)(*(p+i));
#ifdef  DEBUG
		AccFloat[i]=(AccFloat[i]*4.8*2/1023-2.5)/1.2;
#else
		AccFloat[i]=(AccFloat[i]*4.8/1023-2.5)/1.2;
#endif
//		if(AccFloat[i]>=1||AccFloat[i]<=-1)
//			return 0;
	}
	for(i=0;i<qnm;i++)						//建立法方程
	{
		for(j=0;j<qnm;j++)
		{
			qnnn[i][j]=0;
			for(k=0;k<NO;k++)
			{
				DISK_Read((DISK_BUFF_ADDR+k*56+i*4),(Uint8 *)&qnx[0],4);      //qna[k][i]
				DISK_Read((DISK_BUFF_ADDR+k*56+j*4),(Uint8 *)&qnx[1],4);      //qna[k][j]
				////	qnnn[i][j]=qnnn[i][j]+qna[k][i]*qna[k][j];
				qnnn[i][j]=qnnn[i][j] + qnx[0]*qnx[1];
			}
		}
		qnnn[i][j] = 0;
		for(k=0;k<NO;k++)
		{
			DISK_Read((DISK_BUFF_ADDR+k*56+i*4),(Uint8 *)&qnx[0],4);      //qna[k][i]
			qnnn[i][j]=qnnn[i][j]+qnx[0]*AccFloat[k];
			////qnnn[i][j]=qnnn[i][j]+qna[k][i]*AccFloat[k];
		}
	}
	for(i=0;i<qnm;i++)
	{
		for(j=qnm;j>=i;j--)
		{
			if(qnnn[i][i]==0)
				return 0;
			qnnn[i][j]=qnnn[i][j]/qnnn[i][i];
		}
		for(j=i+1;j<qnm;j++)
		{
			for(k=qnm;k>=i;k--)
			{
				qnnn[j][k]=qnnn[j][k]-qnnn[i][k]*qnnn[j][i];
			}
		}
	}
	for(i=qnm-1;i>=0;i--)
	{
		qnx[i]=qnnn[i][qnm];
		for(j=qnm-1;j>i;j--)
		{
			qnx[i]=qnx[i]-qnx[j]*qnnn[i][j];
		}
	}
	for(i=0;i<NO;i++)
	{
		AccFloat[i] = 0;
		for(j=1;j<QNI;j++)
		{
			AccFloat[i]=AccFloat[i]-qnx[j*2-1]*sin(qnw*i*j)/pow(j,2)-qnx[j*2]*cos(qnw*i*j)/pow(j,2);
		}
	}
	// transform displacement to 0-255
	max = min = AccFloat[cycle];
	for(i=cycle;i<(2*cycle);i++)
	{
		if(AccFloat[i]>max)
			max = AccFloat[i];
		if(AccFloat[i]<min)
			min = AccFloat[i];
	}
	stroke = pow((cycle*ParDat.TimeUnit)/(40*3.1415926),2)*(max-min)*9.8;
	if(qnx[0]>0&&qnx[0]<2)
		stroke = stroke / qnx[0];
	if(stroke<0.5||stroke>10)	return 0;	// No such well that stroke<0.5M or >10M
	stroke *= 1000;
	//strokeCal = (Uint8)stroke;
	MEM_set(GatDat->AccBuff,0,NO);
	for(i=cycle;i<(2*cycle);i++)
	{
		GatDat->AccBuff[i-cycle]=(Uint8)((AccFloat[i]-min)*255/(max-min));
	}  
	return 1;   
}
/*
*****************************************************************************
**函 数 名：void BiaoZhunHua(Uint8 n)
**功能描述：
**输    入：Uint8 n 
**输    出：
*****************************************************************************
*/
void BiaoZhunHua(Uint8 n)
{
	Uint8 xdata t,dif,pos;
    Uint16 xdata i; 
	t = n;
#ifdef	DEBUG
	while(t<119)
#else
	while(t<POINTS_NUM)
#endif
	{
		dif = 0;
		for(i=0;i<(t-1);i++)           //找相差最大的数据序号
		{
			if(abs(Msg.Weiy[i]-Msg.Weiy[i+1]) > dif)
			{
				dif = abs(Msg.Weiy[i]-Msg.Weiy[i+1]);
				pos = i;
			}
		}
		for(i=0;i<(t-pos);i++)       //数据从第pos个点开始后移一个数据长度，最后的数据抛弃
		{
			Msg.Load[t-i]=Msg.Load[t-i-1];
			Msg.Weiy[t-i]=Msg.Weiy[t-i-1];
		}
		i = (Msg.Load[pos]+Msg.Load[pos+2]);   //将相差最大的两个数的平均值插到两值之间
		Msg.Load[pos+1] = (BYTE)(i/2);
		i = (Msg.Weiy[pos]+Msg.Weiy[pos+2]);
		Msg.Weiy[pos+1] = (BYTE)(i/2);
		t ++;
	}
}
/*
*****************************************************************************
**函 数 名：void ReGather(BYTE period)
**功能描述：调整数据采集时间间隔  用GatDat->TempLoad[0]作缓存
**输    入： 
**输    出：
*****************************************************************************
*/
void ReGather(Uint8 period)
{
	SysDat.NumRegather++;
	if(period)
	{
		GatDat->TempLoad[0] = period*ParDat.TimeUnit;
		if(ParDat.TimeUnit==((BYTE)(GatDat->TempLoad[0]/96)+1))
			ParDat.TimeUnit = (BYTE)(GatDat->TempLoad[0]/96);
		else
			ParDat.TimeUnit = (BYTE)(GatDat->TempLoad[0]/96)+1;
	}
	else
	{
		ParDat.TimeUnit += 2;
	}
	SysDat.flagGather = 2;
	SetMask(SysDat.Flag,SYS_BE_WORKING);
	
}
/*
*****************************************************************************
**函 数 名：void MakeFormatMsg(void)
**功能描述：打包数据
**输    入： 
**输    出：
*****************************************************************************
*/
void MakeFormatMsg(void)
{
	Msg.Head[0] = '*';		//"*"
	Msg.Head[1] = '*';		//"*"
	Msg.length	  	= sizeof(Msg);
	Msg.deviceNo  	= ParDat.DeviceNo;
	Msg.zxh         = 0;		//10
	Msg.voltage   	= ParDat.voltage;
	Msg.temperature	= ParDat.temperature;
	Msg.spaceTime 	= ParDat.SpaceTime;								// unit : minute
	Msg.timeAD	  	= ParDat.TimeUnit;
}
/*
*****************************************************************************
**函 数 名：void CompleteMsg(void)
**功能描述： 数据发送前对数据进行完善（设置信号强度和校验，信号强度需连接GPRS后才能知道）
**输    入： 
**输    出：
*****************************************************************************
*/
void CompleteMsg(void)
{
	if(ParDat.csqGPRS>0 && ParDat.csqGPRS<32)
	{
		Msg.csq	= ParDat.csqGPRS | 0x80;
	}
	else
	{
		Msg.csq			= 0x80;
	}
	Msg.checkSum  = MsgCheckSum((BYTE*)(&Msg),Msg.length-1);
	msgReady = 1;  
}
/*
*****************************************************************************
**函 数 名：void MakeOldFormatMsg(void)
**功能描述：打包历史数据
**输    入： 无
**输    出： 无
*****************************************************************************
*/
void MakeOldFormatMsg(void)
{
	static Uint8 seq = 0;
	DISK_Read(DISK_OURDATA_ADDR + 134 ,Msg.Weiy,POINTS_NUM);
    MakeFormatMsg();
	Msg.seq		  	= seq++;
	Msg.mode        = 5;		//9
	/*if(ParDat.csqGPRS>0 && ParDat.csqGPRS<32)      
	{
		Msg.csq	= ParDat.csqGPRS | 0x80;
	}
	else
	{
		Msg.csq			= 0x80;
	}*/
	MEM_cpy(Msg.Load,GatDat->LoadBuff,TOTAL);
	//Msg.checkSum  = MsgCheckSum((BYTE*)(&Msg),Msg.length-1);

	msgReady = 1;    
}
/*
*****************************************************************************
**函 数 名：void MakeNewFormatMsg(BYTE period)
**功能描述：打包新数据
**输    入：Uint8 period 
**输    出： 无
*****************************************************************************
*/
void MakeNewFormatMsg(Uint8 period)
{
	static Uint8 seq = 0;
	//Uint8 xdata Buff[120];
	MEM_set(Msg.Head,0,sizeof(MESSAGE_DATA));
    MakeFormatMsg();
	Msg.seq	= seq++;
	/*if(ParDat.csqGPRS>0 && ParDat.csqGPRS<32)
	{
		Msg.csq	= ParDat.csqGPRS | 0x80;     //最高位置1，新数据格式
	}
	else
	{
		Msg.csq	= 0x80;
	} */
	Msg.mode = 6;		//9
	MEM_cpy(Msg.Load,(Uint8 *)GatDat->LoadBuff[period],period);
	MEM_cpy(Msg.Weiy,GatDat->AccBuff,period);
	BiaoZhunHua(period);

	/*MEM_cpy(Buff,Msg.Weiy,120);
	MEM_cpy(Msg.Weiy,Buff,120); */

	//Msg.checkSum  = MsgCheckSum((BYTE*)(&Msg),Msg.length-1);

	msgReady = 1;  
}
/*
*****************************************************************************
**函 数 名：void MakeMessage(void)
**功能描述：数据处理
**输    入：  无
**输    出：  无
*****************************************************************************
*/
void MakeMessage(void)
{
	Uint8 xdata period = 0;
	Uint8 xdata statusWell;
	if(dataReady)
	{
		statusWell = JudgeWellStatus(GatDat->LoadBuff,TOTAL);    //判断负载（载荷）
		if(!statusWell)
		{
			period = GetCycle(GatDat->LoadBuff,TOTAL);
			if(period<70||period>120)
			{
				if(ParDat.TimeUnit <= 15 && SysDat.NumRegather<5)	
				{
					ReGather(period);
				}
				else 
				{					
					statusWell = 4;
					if(SysDat.NumRegather >= 5)	statusWell = 5;
					SysDat.NumRegather = 0;
					MakeOldFormatMsg();
				}
			}
			else
			{
				SysDat.NumRegather = 0;
//				if(timeUnit!=XBYTE[PERIOD_ADDR])
//					SaveToFlash(PERIOD_ADDR,0,&timeUnit,1);
				if(GetDisplacement(GatDat->AccInt,period))
				{
					MakeNewFormatMsg(period);
				}
				else
				{
					statusWell = 5;
					MakeOldFormatMsg();
				}
			}
		}
		else
		{
			MakeOldFormatMsg();
		}
		DISK_Write(DISK_OURDATA_ADDR,Msg.Head,sizeof(MESSAGE_DATA));
		dataReady = 0;
	} 
}
