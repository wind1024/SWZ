/***********************************************************************************
**文件说明：系统相关操作文件                                                                
**文件名称：sys_about.c                                                      
**创建日期：2013                                                            
**创 建 人：XZY                                                                   
**---------------------------------------------------------------------------------*
**修改日期：                                                                       
**修改说明：                                                                       
***********************************************************************************/
#include "C8051F930_lib.h"
#include "memory.h"
#include "pin_define.h"
#include "SWSerial.h"
#include "diskio.h"
#include "Acceleration.h"
#include "calculate.h"
#include "communication.h"
#include "ZigBee.h"

/***********************************************************************************/
/*函数名称：void SYS_Delay(Uint8 count)                                           */
/*函数功能：毫秒延时                                                                   */
/*入口参数：Uint8 count                                                            */
/*返回参数：无                                                                     */
/***********************************************************************************/
void SYS_Delay(Uint8 count)
{
	Uint8 a,b;
	for(;count > 0 ;count --)
	{
    	for(b=4;b>0;b--)				 //1ms误差 -0.651041666667us
        	for(a=190;a>0;a--);
	}
}
/*
*****************************************************************************
**函 数 名：void InitTimer3(void)
**功能描述： 定时器3初始化
**输    入： 
**输    出：
*****************************************************************************
*/
void InitTimer3(void)
{
	DisableTimer3();
	TIME_Delay(3,10);
}
/*
*****************************************************************************
**函 数 名：void GatherVariable(void) 
**功能描述： 数据采集相关参数初始化
**输    入： 
**输    出：
*****************************************************************************
*/
void GatherVariable(void)
{
	GSM_RSTON();                //GPRS为复位状态
	POWERON();                  //开载荷电源
	G_POWERON();                //开加速度传感器电源
	SysDat.pADC = 0;	
	SysDat.channelADC = 0xff;
	SysDat.timeLeft = 0;
	SysDat.pFilter = 0;
	MEM_set(GatDat->LoadBuff,0,sizeof(GATHER_DAT));
	if(SysDat.NumRegather == 0)        //初次数据采集
	{
		SysDat.flagGather = 1;
	}
}
/*
*****************************************************************************
**函 数 名：void GatherData(void)
**功能描述：数据采集
**输    入：无
**输    出：无
*****************************************************************************
*/
void GatherData(void)
{
	//ACCEL_Init();
	if(SysDat.flagGather == 0)     //如果不要数据采集直接退出
	{
		return;
	}
	if(ReadMask(SysDat.Flag,SYS_BE_WORKING) == SYS_BE_WORKING)     //开始数据采集
	{
		ClearMask(SysDat.Flag,SYS_BE_WORKING);
		InitTimer3();
		Timer3IntSwitchOn();	
		GatherVariable();
	}		
	if(SysDat.flagGather == 1)
	{		
		//Timer3Open();		
		while(SysDat.flagGather != 2)             //等待电压和温度数据采集完成
		{
			if(!SysDat.timeLeft)
			{		
				if(!AD0BUSY)
				{
					ADC0_Converx(VOLTAGE);
					ADC0_Converx(TEMPERATURE);
					if(SysDat.pFilter>=5)
					{
						SysDat.pFilter = 0;
						ParDat.voltage = (Uint8)(Filter(GatDat->TempLoad,5)>>2);	  //取ADC高8位
						ParDat.temperature  = (Uint8)(Filter(GatDat->TempAcc,5)>>2);	  //取ADC高8位
						SysDat.flagGather = 2;
						break;	
					}
					SysDat.timeLeft = 10;
					Timer3Open();
				}
			}
			PCON |= 0x01;                              //空闲方式（省电），由Timer3中断唤醒
		}
	}
	if(SysDat.flagGather == 2)
	{
		if(ReadMask(SysDat.Flag,ACC_INT_MASK) != ACC_INT_MASK)     //加速度模块是否初始化
		{
			ACCEL_Init();
		}
		//Timer3Open();		
		if(SysDat.pADC<240)                            //等待载荷和加速度数据采集完成
		{
			if(!SysDat.timeLeft)
			{
				if(!AD0BUSY)
				{
					ADC0_Converx(LOAD);
					if(ReadMask(SysDat.Flag,ACC_INT_MASK) == ACC_INT_MASK)
					{
						MMA8452_Active();//开启加速度传感器
						while(ReadMask(SysDat.Flag,ACC_DATA_MASK) != ACC_DATA_MASK); //等待加速度转换读取完成					
						ClearMask(SysDat.Flag,ACC_DATA_MASK);
					}
					SysDat.pFilter ++;
					if(SysDat.pFilter>=5)
					{
						SysDat.pFilter = 0;
						GatDat->LoadBuff[SysDat.pADC] = (Uint8)(Filter(GatDat->TempLoad,5)>>2);	  //取ADC高8位
						GatDat->AccInt[SysDat.pADC] = Filter(GatDat->TempAcc,5);
						GatDat->AccBuff[SysDat.pADC] = (Uint8)(GatDat->AccInt[SysDat.pADC]>>2);	  //取ADC高8位
						SysDat.pADC++;						
					}	
					//ADC0_Converx(ACCELERATION);
					SysDat.timeLeft = ParDat.TimeUnit;         //新采样间隔
					Timer3Open();                              //开启定时器					
				}
			}
			PCON |= 0x01;			        //空闲方式（省电），由Timer3中断唤醒
		}
		else
		{
			ACCEL_OFF();                      //关闭加速度传感器			
			DisableTimer3();
			SysDat.flagGather = 0;
			dataReady = 1;
			ADC_Over();
			G_POWEROFF();
			POWEROFF();
			Flag_NetOk = 0;
			ClearMask(SysDat.Flag,ACC_INT_MASK);
		}
	}
	if(SysDat.flagGather>2)
	{
		SysDat.flagGather = 0;
	}
}

/*
***********************************************************************************
**函数名称：void ParaInit(void)                                                                        
**函数功能：系统参数初始化                                           
**入口参数： 无                                                        
**返回参数： 无                                                                 
***********************************************************************************
*/
void ParaInit(void)
{
	if(!PC_SET)
	{
		MEM_set((Uint8 *)& (ParDat.SpaceTime),0,sizeof(PARAMETER));
		DISK_Write(DISK_PARA_ADDR,(Uint8 *)&(ParDat.SpaceTime),(sizeof(PARAMETER)));
	}		
	DISK_Read(DISK_PARA_ADDR,(Uint8 *)&(ParDat.SpaceTime),(sizeof(PARAMETER)));
	if((ParDat.MemInitFlag&0xF0) != MEM_INIT_FLAG)
	{		
		MEM_set((Uint8 *)& (ParDat.SpaceTime),0,sizeof(PARAMETER));
		ParDat.SpaceTime = 10;
		MEM_cpy(ParDat.DeviceIpAddr,BakIp,4);
		ParDat.Port = BakPort;
		ParDat.DeviceNo = BakDeviceNo;
		ParDat.TimeUnit = 1;
		ParDat.MemInitFlag =  MEM_INIT_FLAG; 
	}
	/*MEM_set(Msg.Head,0x55,sizeof(MESSAGE_DATA));
	DISK_Write(DISK_BUFF_ADDR,Msg.Head,DATA_LONG);
	MEM_set(Msg.Head,0,sizeof(MESSAGE_DATA));
	DISK_Read(DISK_BUFF_ADDR,Msg.Head,DATA_LONG); */
	/*DISK_Write(DISK_PARA_ADDR,(Uint8 *)&(ParDat.SpaceTime),(sizeof(PARAMETER)));
	MEM_set((Uint8 *)& (ParDat.SpaceTime),0,sizeof(PARAMETER));
	DISK_Read(DISK_PARA_ADDR,(Uint8 *)&(ParDat.SpaceTime),(sizeof(PARAMETER))); */
	MEM_set((Uint8 *)& (SysDat.Flag),0,sizeof(SYS_MODEDATA));
	SysDat.PointAddr = ParDat.RecCount * DATA_LONG + DISK_DATA_SADDR;
}
/*
*****************************************************************************
**函 数 名：void InitVariable(void)
**功能描述：参数初始化
**输    入： 无
**输    出： 无
*****************************************************************************
*/
void InitVariable(void)
{
	ParaInit();
	InitTimer3();
	SysDat.TimerTest = 10;          //等待10S后设置ZigBee
	SetMask(SysDat.Flag,TIME_ON_MASK);
	EnableTimer3();
	while(SysDat.TimerTest);
	DisableTimer3();
	ClearMask(SysDat.Flag,TIME_ON_MASK);
	ZigBeeInit();
	TIME_Delay(2,20);
	EnableTimer2();
	SetMask(SysDat.Flag,SYS_BE_WORKING);	
}

/***********************************************************************************/
/*函数名称：void SYS_Init(void)                                                    */
/*函数功能：系统初始化                                                             */
/*入口参数：无                                                                     */
/*返回参数：无                                                                     */
/***********************************************************************************/
void SYS_Init(void)
{
	CloseWT();
	//数据划分
	GatDat = (GATHER_DATP)(XGDat);
	LinkDat = (LINK_DATP)(XGDat);
	BufDat = (BUF_DATP)(XGDat + 100);
	IOPortInit();                          //IO口初始化
	SysClkInit(CLK_EXT_OSC,OSC_SYSCLK_Div1,EXT_OSC_CRY,EXT_OSC_FREQUENCY_RANGE7);    //系统时钟初始化
	Timer23Init(2,TIMER23_CLKSOURCE_CLK_0);		   //定时器2初始化
	Timer23Init(3,TIMER23_CLKSOURCE_CLK_0);        //定时器3初始化
	UART0Init(UART0_MODE1);                        //硬件串口初始化
	SW_UART_INIT();                                //软件串口初始化
	SW_UART_ENEABLE();
	SYS_Delay(200);
	GlobleIntSwitchOn();							//全局中断开
}
/*
*****************************************************************************
**函 数 名：void Sleep(void)
**功能描述：系统进休眠模式
**输    入： 
**输    出：
*****************************************************************************
*/
void Sleep(void)
{
	//Send_Over();
	GlobleIntSwitchOn();				//全局中断开
	RTC_Init();                         //初始化SmaRTClock
   	RTC_SetCurrentTime(0);				//设置SmaRTClock初值
	RTC_WriteAlarm(ParDat.SpaceTime*ONE_MIN_TICKS);	   //设置SmaRTClock告警值
	if((ReadMask(SysDat.Flag,SYS_HOST_MASK)== SYS_HOST_MASK)&&(ReadMask(SysDat.Flag,ZB_ONLINE_MASK) == ZB_ONLINE_MASK))  //协调器Coordinator
	{
		RTC0CN_SetBits(RTC0TR+RTC0AEN);		// Enable Counter, Alarm
		RTC0AIntSwitchOn();
		LPM_Init();                         // Initialize Power Management
		LPM_Enable_Wakeup(RTC);	 										  
		//RTC_Alarm = 1;                      // Set the RTC Alarm Flag on startup
	}
	else
	{
		Timer2Off();
		RTC0CN_SetBits(RTC0TR+RTC0AEN+ALRM);// Enable Counter, Alarm, and Auto-Reset*/
		RTC0AIntSwitchOn();
		LPM_Init();                         // Initialize Power Management
		LPM_Enable_Wakeup(RTC);	 										  
		//RTC_Alarm = 1;                      // Set the RTC Alarm Flag on startup		
		LPM(SLEEP);
		SetMask(SysDat.Flag,SYS_BE_WORKING);
		SYS_Delay(200);
	}  		
}
/*
*****************************************************************************
**函 数 名：void ConnectGprsNet(void)
**功能描述：连接GPRS
**输    入： 无
**输    出： 无
*****************************************************************************
*/
void ConnectGprsNet(void)
{
	if(msgReady&&!Flag_NetOk)/////&&Init_Flag&&!debugMode)
	{
		POWERON();
		TestAndConnect(2);
		if(!Flag_NetOk)
			centerAnswer = 1;			// to sleep
	} 
}
/*
*****************************************************************************
**函 数 名：void DetectCommand(void)
**功能描述：分析中心反馈数据
**输    入：  无
**输    出：  无
*****************************************************************************
*/
void DetectCommand(void)           //分析中心反馈数据
{
	BYTE xdata i;//,Buff[4];
	//if(UDP_Received && !isDisposingCom)
	if(UDP_Received)
	{
		UDP_Received = 0;
		//isDisposingCom = 1;
		if(LinkDat->UDP_Data[0]=='$' && LinkDat->UDP_Data[1]=='$')							// data from center
		{
			LinkDat->UDP_Data_Len = LinkDat->UDP_Data[2];
			if(LinkDat->UDP_Data_Len&&(*((WORD*)(&LinkDat->UDP_Data[3])))==ParDat.DeviceNo)
			{
				if(Msg.seq==LinkDat->UDP_Data[5])									//shake hand--ok
				{
					centerAnswer = 1;
					SysDat.TimeResend = 0;
				}
				if(MsgCheckSum(LinkDat->UDP_Data,LinkDat->UDP_Data_Len-1)==LinkDat->UDP_Data[LinkDat->UDP_Data_Len-1])      
				{
					if(LinkDat->UDP_Data_Len>ANSWER_BYTE && (LinkDat->UDP_Data_Len-ANSWER_BYTE)%2==0)
					{
						for(i=(ANSWER_BYTE-1);i<LinkDat->UDP_Data_Len-1;i+=2)
						{
							switch(LinkDat->UDP_Data[i])
							{
								case 0x01:                //修改发送间隔
									if(LinkDat->UDP_Data[i+1])
									{
										//spaceTime = LinkDat->UDP_Data[i+1];
										ParDat.SpaceTime = LinkDat->UDP_Data[i+1];
										SetMask(SysDat.Flag,CAL_CHAN_MASK);
										//SaveToFlash(PARAM_ADDR,0,&UDP_Data[i+1],1);
									}
									break;
								case 0x02:                 //修改载荷数据采集时间
									if(LinkDat->UDP_Data[i+1]>0 && LinkDat->UDP_Data[i+1]<101)
									{
										ParDat.TimeUnit = LinkDat->UDP_Data[i+1];
										SetMask(SysDat.Flag,CAL_CHAN_MASK);
										//memset(Buff,0,4);
										//SaveToFlash(DIST_ADDR,0,Buff,4);
										//Buff[0] = 5;
										//SaveToFlash(PERIOD_ADDR,0,Buff,1);
									}
									break;
								case 0x03:                   //修改报文数据采集格式
									//if(UDP_Data[i+1]<=MAX_TIMEUNIT && UDP_Data[i+1]>0)
									{
										//SaveToFlash(PERIOD_ADDR,0,&UDP_Data[i+1],1);
										//i = 0;
										//SaveToFlash(PARAM_ADDR,1,&i,1);
									}
									break;
								case 0x21:                  //修改中心IP（4），端口号（2），本部分参数异或校验（1）
									if(MsgCheckSum(LinkDat->UDP_Data+i+1,6)==LinkDat->UDP_Data[i+7])
									{
										//SaveStringFlash(StartAddress,CENTER_IP_POS,UDP_Data+i+1,6);
										MEM_cpy(ParDat.DeviceIpAddr,&LinkDat->UDP_Data[i+1],4);
										MEM_cpy((Uint8 *)&ParDat.Port,&LinkDat->UDP_Data[i+5],2);
										SetMask(SysDat.Flag,CAL_CHAN_MASK);
										i += 6;
									}
									break;
								case 0x22:                  //抽油井：修改同伴IP（4），端口号（2），本部分参数异或校验（1）
									if(MsgCheckSum(LinkDat->UDP_Data+i+1,6)==LinkDat->UDP_Data[i+7])
									{
										//SaveStringFlash(StartAddress,TARGET_IP_POS,UDP_Data+i+1,6);
										//i += 6;
									}
							}
						}
					}
				}
			}
			else
			{
				ClearMask(SysDat.Flag,ZB_ONLINE_MASK);       //先清除标志，做后续设置
				ZigBeeSend(LinkDat->UDP_Data,LinkDat->UDP_Data_Len);
				SetMask(SysDat.Flag,ZB_ONLINE_MASK);        //反馈信息下发完成，置ZigBee在线标志
				centerAnswer = 1;
				SysDat.TimeResend = 0;
			}
		}
		//isDisposingCom = 0;
	} 
}
/*
*****************************************************************************
**函 数 名：void SendDataToCenter(void)
**功能描述： 发送数据
**输    入： 无
**输    出： 无
*****************************************************************************
*/
void SendDataToCenter(void)
{	
	if(msgReady&&Flag_NetOk)
	{
		SEND_DATA_WAVECOM((BYTE*)(&Msg),Msg.length);
		flagResend = 1;
		SysDat.TimeResend = 2;
		//timeOneSec = 15;			//15 seconds
		SysDat.TimerTest = 15;
		SetMask(SysDat.Flag,TIME_ON_MASK);
		EnableTimer3();
		msgReady = 0;
	}
	if(flagResend&&!SysDat.TimerTest&&Flag_NetOk)
	{
		if(SysDat.TimeResend )
		{
			SEND_DATA_WAVECOM((BYTE*)(&Msg),Msg.length);
			flagResend = 1;
			SysDat.TimeResend --;
			//timeOneSec = 15;		//15 seconds
			SysDat.TimerTest = 15;
			SetMask(SysDat.Flag,TIME_ON_MASK);
			EnableTimer3();
		}
		else
		{
			centerAnswer = 1;
			flagResend = 0;
		}
		
	}
	/*if(centerAnswer)
	{
		{
			centerAnswer = 0;
			flagResend = 0;
			//timeOneSec = 0;		
			SysDat.TimeResend = 0;
			SysDat.TimerTest = 0;
			DisableTimer3();
			ClearMask(SysDat.Flag,TIME_ON_MASK);

			Sleep();
		}
	}*/  
}
/*
*****************************************************************************
**函 数 名： void SendRouterDatToCenter(void)
**功能描述： 发送子节点数据到服务器并将服务器应答下发 (此过程禁止接收路由上传的信息)
**输    入： 
**输    出：
*****************************************************************************
*/
void SendRouterDatToCenter(void)
{
	if(ParDat.RecCount)
	{
		msgReady = 1;
		ConnectGprsNet();
		if(centerAnswer)
		{
			return;
		}
		SysDat.BuffAddr = SysDat.PointAddr;
		for(ParDat.RecCount ; ParDat.RecCount > 0; ParDat.RecCount--)
		{			
			SysDat.BuffAddr = SysDat.BuffAddr - DATA_LONG;
			DISK_Read(SysDat.BuffAddr,Msg.Head,DATA_LONG-21);		//不读 SIMID[20]和checkSum
			CompleteMsg();
			while(!centerAnswer)
			{
				SendDataToCenter();
				DetectData_WaveCom();
				DetectCommand();
			}
			centerAnswer = 0;
			flagResend = 0;
			SysDat.TimeResend = 0;
			SysDat.TimerTest = 0;
			DisableTimer3();
			ClearMask(SysDat.Flag,TIME_ON_MASK);			
		}
		SysDat.PointAddr = SysDat.BuffAddr;
	}
}
/*
*****************************************************************************
**函 数 名：void RouterDatProcess(void)
**功能描述： 路由上传数据处理
**输    入： 
**输    出：
*****************************************************************************
*/
void RouterDatProcess(void)
{	
	if(ZB_Rce_Complete)
	{
		ZB_Rce_Complete = 0;
		DISK_Write(SysDat.PointAddr,BufDat->Dat,DATA_LONG);
		ParDat.RecCount ++;
		SysDat.PointAddr = SysDat.PointAddr + DATA_LONG;
		/*if(ReadMask(SysDat.Flag,SEC_COUNT_MASK) != SEC_COUNT_MASK)
		{
			Timer2Open();
			SysDat.SecCount = 180;    //3分钟
			SetMask(SysDat.Flag,SEC_COUNT_MASK);
		} */
		SysDat.SecCount = 0;
	}
	if(ParDat.RecCount >= 5 || (!SysDat.SecCount))       //数据5组以上或3分钟时间到，将数据上传
	{		
		Timer2Off();
		SysDat.SecCount = 180;
		SendRouterDatToCenter();		
		ClearMask(SysDat.Flag,SEC_COUNT_MASK);
		USER_LED = 0;
		POWEROFF();                         //关闭5V和3.6V电源
		Flag_NetOk = 0;
	}
	/*if(ReadMask(SysDat.Flag,SEC_COUNT_MASK) != SEC_COUNT_MASK)
	{
		Timer2Open();
		SysDat.SecCount = 180;    //3分钟
		SetMask(SysDat.Flag,SEC_COUNT_MASK);
	} */
	
}
/*
*****************************************************************************
** 函 数 名: void SYS_Daemon(void) 
** 功能描述: 系统进程
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/
void SYS_Daemon(void)
{
	USER_LED = 1;
	SysDat.flagGather = 1;
	Timer2Open();
	SysDat.MainLoopError = 0;
	SW_UART_DISABLE();
	DISABLE_UART0();
	while(SysDat.flagGather)
	{
		GatherData();            //数据采集 dataReady = 1,SysDat.flagGather = 0
		SysDat.MainLoopError = 0;
		MakeMessage();           //数据处理 msgReady = 1,dataReady = 0
	}
	DISK_Write(DISK_OURDATA_ADDR,Msg.Head,DATA_LONG);
	//DISK_Read(DISK_OURDATA_ADDR,Msg.Head,DATA_LONG);
	SysDat.MainLoopError = 0;
	if((ReadMask(SysDat.Flag,SYS_HOST_MASK) == SYS_HOST_MASK) || (ReadMask(SysDat.Flag,ZB_ONLINE_MASK) != ZB_ONLINE_MASK))
	{
		/*DISK_Write(SysDat.PointAddr,Msg.Head,DATA_LONG);
		ParDat.RecCount ++;
		SysDat.PointAddr = SysDat.PointAddr + DATA_LONG;
		if(ReadMask(SysDat.Flag,SEC_COUNT_MASK) != SEC_COUNT_MASK)     //主机数据采集前没收到从机数据，等待3分钟
		{
			Timer2Open();
			SysDat.SecCount = 180;    //3分钟
			SetMask(SysDat.Flag,SEC_COUNT_MASK);
		}
		else         //主机数据采集前收到过从机数据立即处理
		{
			SysDat.SecCount  = 0;
		} */
		//SW_UART_ENEABLE();
		ConnectGprsNet();
		CompleteMsg();
		while(!centerAnswer)
		{
			SendDataToCenter();
			DetectData_WaveCom();
			DetectCommand();
		}
		centerAnswer = 0;
		flagResend = 0;
		SysDat.TimeResend = 0;
		SysDat.TimerTest = 0;
		DisableTimer3();
		ClearMask(SysDat.Flag,TIME_ON_MASK);
		MEM_set(BufDat->Dat,0,sizeof(BUF_DAT));
		POWEROFF();                         //关闭5V和3.6V电源
		Flag_NetOk = 0;
		SW_UART_ENEABLE();		
	}
	else 
	{
		CompleteMsg();
		SW_UART_ENEABLE();
		for(SysDat.TimeResend = 0;SysDat.TimeResend < 3;SysDat.TimeResend++)   //最多发送3次每次间隔200秒
		{
			ZigBeeSend(Msg.Head,sizeof(MESSAGE_DATA));
			MEM_set(BufDat->Dat,0,sizeof(BUF_DAT));
			SysDat.MainLoopError = 0;
			SysDat.SecCount = 200;    //200秒
			SetMask(SysDat.Flag,SEC_COUNT_MASK);
			while((!ZB_Rce_Complete) && (SysDat.SecCount));       //等待协调应答	
			ClearMask(SysDat.Flag,SEC_COUNT_MASK);
			SysDat.MainLoopError = 0;
			if(ZB_Rce_Complete)
			{
				ZB_Rce_Complete = 0;
				UDP_Received = 1;
				LinkDat->UDP_Data = BufDat->Dat;
				DetectCommand();
				break;
			}
		}		
	}
	USER_LED = 0;	
}
/*
*****************************************************************************
** 函 数 名: void RTC_ALRM(void) interrupt RTC0_ALARM_PRIOR_ORDER
** 功能描述: RTC告警中断
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
*****************************************************************************
*/ 
//INTERRUPT(RTC_ALRM,INTERRUPT_RTC0ALARM)
void RTC_ALRM(void) interrupt RTC0_ALARM_PRIOR_ORDER
{
	GlobleIntSwitchOff();		//全局中断关	
	//USER_LED = ~USER_LED;
	RTC_SetCurrentTime(0);				// 
	RTC_WriteAlarm(ParDat.SpaceTime*ONE_MIN_TICKS);//WAKE_INTERVAL_TICKS);// Set the Alarm Value
	SetMask(SysDat.Flag,SYS_BE_WORKING);
	GlobleIntSwitchOn();		//全局中断开
}

















	