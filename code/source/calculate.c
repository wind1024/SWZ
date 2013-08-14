/*
**********************************************************************************
**�ļ�˵�����������                                                            
**�ļ����ƣ�calculate.c                                                   
**�������ڣ�2013.                                                 
**�� �� �ˣ�XZY                                                                 
**---------------------------------------------------------------------------------*
**�޸����ڣ�2013.07.16                                                                    
**�޸�˵��������ǰ�����㷨�������������γɴ��ļ������ڴ�ռ�ù�������Ż�������δ�ı���ʵ��                                                                    
**********************************************************************************
*/
#include "user_type.h"
#include <math.h>
#include "memory.h"
#include "diskio.h"

//----------------------------------------------------------------------------
//˵�����������ݴ���
//----------------------------------------------------------------------------
/*
*****************************************************************************
**�� �� ����Uint16 Filter(Uint16 *p,Uint8 n)
**�������������������ݵ��м�ֵ(��ֵ�˲�)
**��    �룺���������׵�ַUint16 *p�����鳤��Uint8 n 
**��    �������������м�ֵ
*****************************************************************************
*/
Uint16 Filter(Uint16 *p,Uint8 n)	//	���������ݵ��м�ֵ
{
	Uint8 xdata i,j;
	Uint16 xdata tempData;
	Uint32 xdata temp;
	
	if(n<=1)
		return *p;
	else
	{
		for(j=0;j<n-1;j++)									// ��С��������
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
**�� �� ����Uint8 MsgCheckSum(Uint8 *addr,Uint16 length)
**�������������У���
**��    �룺Uint8 *addr,Uint16 length 
**��    ����8λ���У���
*****************************************************************************
*/
Uint8 MsgCheckSum(Uint8 *addr,Uint16 length)//���У��
{
	Uint8 xdata sum = 0;
	for(;length>0;length--)
		sum ^= *((Uint8*)addr++);
	return sum;
}
/*
*****************************************************************************
**�� �� ����Uint8 MinCalculate(Uint8 *p,Uint16 n)
**��������������Сֵ
**��    �룺Uint8 *p,Uint16 n 
**��    ������Сֵ
*****************************************************************************
*/
Uint8 MinCalculate(Uint8 *p,Uint8 n)//����Сֵ
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
**�� �� ����Uint8 MaxCalculate(Uint8 *p,Uint8 n)
**���������������ֵ
**��    �룺Uint8 *p,Uint8 n 
**��    �������ֵ
*****************************************************************************
*/
Uint8 MaxCalculate(Uint8 *p,Uint8 n)//�����ֵ
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
**�� �� ����Float32 Average(Uint8 *p,Uint8 n)
**�������������ֵ
**��    �룺Uint8 *p,Uint8 n 
**��    ����ƽ��ֵ
*****************************************************************************
*/
Float32 Average(Uint8 *p,Uint8 n)//���ֵ
{
	Float32 xdata sum = 0;
	Uint8 xdata i;
	for(i=0;i<n;i++)
		sum += *(p+i);
	return (sum/n);
}
/*
*****************************************************************************
**�� �� ����Uint8 SerialLess(Uint8 *p,Uint8 n)
**����������ͳ��������С��ƽ��ֵ�����������ֵ������
**��    �룺Uint8 *p,Uint8 n
**��    �������
*****************************************************************************
*/
Uint8 SerialLess(Uint8 *p,Uint8 n)         //ͳ��������С��ƽ��ֵ�����������ֵ������
{
	Uint8 xdata i,belowCnt,tempBelowCnt,aver;

	belowCnt=tempBelowCnt=0;
	aver = Average(p,n);
	for(i=0;i<n;i++)
	{
		if(*(p+i)<aver)
			tempBelowCnt ++;                             //С��ƽ��ֵ����ͳ��
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
**�� �� ����Uint8 SerialMore(Uint8 *p,Uint8 n)
**����������ͳ�������д���ƽ��ֵ�����������ֵ������
**��    �룺Uint8 *p,Uint8 n 
**��    �������
*****************************************************************************
*/
Uint8 SerialMore(Uint8 *p,Uint8 n)      //ͳ�������д���ƽ��ֵ�����������ֵ������
{
	Uint8 xdata i,overCnt,tempOverCnt,aver;

	overCnt=tempOverCnt=0;
	aver = Average(p,n);
	for(i=0;i<n;i++)
	{
		if(*(p+i)>aver)
			tempOverCnt ++;                  //����ƽ��ֵ����ͳ��
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
**�� �� ����void CalculateArea(Uint8 *p,Uint8 n)
**�����������������
**��    �룺Uint8 *p,Uint8 n 
**��    ����
*****************************************************************************
*/
void CalculateArea(Uint8 *p,Uint8 n)    //�������
{
	Uint8 xdata i,j;
	Uint16 xdata temp,sum;	
	Uint8 start = 0;

	for(i=0;i<(TOTAL-n);i++)
	{
		sum = 0;
		for(j=0;j<=n;j++)
		{
			temp = abs(*(p+j+start)-*(p+i+j));//����ֵ
			sum += temp;
		}
		GatDat->Area[i]=sum;
	}
}
/*
*****************************************************************************
**�� �� ����Uint8 FindLeastValue(Uint16 *p,Uint8 n)
**������������Ҫ���ҵ�20�������������������
**��    �룺Uint16 *p,Uint8 n 
**��    ����
*****************************************************************************
*/
Uint8 FindLeastValue(Uint16 *p,Uint8 n)     //��Ҫ���ҵ�20�������������������
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
	for(i=1;i<tempAcc;i++)                                 //��С��������
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
**�� �� ����void DeleteArray(Uint8 len,Uint8 pos)
**����������ɾ���� pos��������������ǰ��һ��λ��
**��    �룺Uint8 len,Uint8 pos 
**��    ����
*****************************************************************************
*/
void DeleteArray(Uint8 len,Uint8 pos)          //ɾ���� pos��������������ǰ��һ��λ��
{
	Uint8 xdata i;
	for(i=0;i<(len-pos-1);i++)
	{
		GatDat->Num[pos+i] = GatDat->Num[pos+i+1];
	}
}
/*
*****************************************************************************
**�� �� ����Uint8 DistanceEqu(Uint8 len)
**����������
**��    �룺Uint8 len 
**��    ����
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
// ˵�����㷨
//---------------------------------------------------------------------------
/*
*****************************************************************************
**�� �� ����Uint8 JudgeWellStatus(Uint8 *p,Uint8 n)
**�����������غ������ж�
**��    �룺Uint8 *p,Uint8 n
**��    ����
*****************************************************************************
*/
Uint8 JudgeWellStatus(Uint8 *p,Uint8 n)          //�غ������ж�
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
	else if((SerialLess(p,n)<MIN_DIF||SerialMore(p,n)<MIN_DIF)||(max-min)<MIN_DIF)     //��Χ����MIN_DIF��5
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
**�� �� ����Uint8 GetCycle(Uint8 *p,Uint8 n)
**����������������
**��    �룺Uint8 *p,Uint8 n
**��    ����
*****************************************************************************
*/
Uint8 GetCycle(Uint8 *p,Uint8 n)                //������
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
**�� �� ����Bool GetDisplacement(Uint16 *p,Uint8 cycle)
**����������
**��    �룺Uint16 *p,Uint8 cycle 
**��    ����
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
	for(i=0;i<qnm;i++)						//����������
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
**�� �� ����void BiaoZhunHua(Uint8 n)
**����������
**��    �룺Uint8 n 
**��    ����
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
		for(i=0;i<(t-1);i++)           //����������������
		{
			if(abs(Msg.Weiy[i]-Msg.Weiy[i+1]) > dif)
			{
				dif = abs(Msg.Weiy[i]-Msg.Weiy[i+1]);
				pos = i;
			}
		}
		for(i=0;i<(t-pos);i++)       //���ݴӵ�pos���㿪ʼ����һ�����ݳ��ȣ�������������
		{
			Msg.Load[t-i]=Msg.Load[t-i-1];
			Msg.Weiy[t-i]=Msg.Weiy[t-i-1];
		}
		i = (Msg.Load[pos]+Msg.Load[pos+2]);   //�����������������ƽ��ֵ�嵽��ֵ֮��
		Msg.Load[pos+1] = (BYTE)(i/2);
		i = (Msg.Weiy[pos]+Msg.Weiy[pos+2]);
		Msg.Weiy[pos+1] = (BYTE)(i/2);
		t ++;
	}
}
/*
*****************************************************************************
**�� �� ����void ReGather(BYTE period)
**�����������������ݲɼ�ʱ����  ��GatDat->TempLoad[0]������
**��    �룺 
**��    ����
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
**�� �� ����void MakeFormatMsg(void)
**�����������������
**��    �룺 
**��    ����
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
**�� �� ����void CompleteMsg(void)
**���������� ���ݷ���ǰ�����ݽ������ƣ������ź�ǿ�Ⱥ�У�飬�ź�ǿ��������GPRS�����֪����
**��    �룺 
**��    ����
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
**�� �� ����void MakeOldFormatMsg(void)
**���������������ʷ����
**��    �룺 ��
**��    ���� ��
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
**�� �� ����void MakeNewFormatMsg(BYTE period)
**�������������������
**��    �룺Uint8 period 
**��    ���� ��
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
		Msg.csq	= ParDat.csqGPRS | 0x80;     //���λ��1�������ݸ�ʽ
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
**�� �� ����void MakeMessage(void)
**�������������ݴ���
**��    �룺  ��
**��    ����  ��
*****************************************************************************
*/
void MakeMessage(void)
{
	Uint8 xdata period = 0;
	Uint8 xdata statusWell;
	if(dataReady)
	{
		statusWell = JudgeWellStatus(GatDat->LoadBuff,TOTAL);    //�жϸ��أ��غɣ�
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
