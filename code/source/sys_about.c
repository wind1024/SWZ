/***********************************************************************************
**�ļ�˵����ϵͳ��ز����ļ�                                                                
**�ļ����ƣ�sys_about.c                                                      
**�������ڣ�2013                                                            
**�� �� �ˣ�XZY                                                                   
**---------------------------------------------------------------------------------*
**�޸����ڣ�                                                                       
**�޸�˵����                                                                       
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
/*�������ƣ�void SYS_Delay(Uint8 count)                                           */
/*�������ܣ�������ʱ                                                                   */
/*��ڲ�����Uint8 count                                                            */
/*���ز�������                                                                     */
/***********************************************************************************/
void SYS_Delay(Uint8 count)
{
	Uint8 a,b;
	for(;count > 0 ;count --)
	{
    	for(b=4;b>0;b--)				 //1ms��� -0.651041666667us
        	for(a=190;a>0;a--);
	}
}
/*
*****************************************************************************
**�� �� ����void InitTimer3(void)
**���������� ��ʱ��3��ʼ��
**��    �룺 
**��    ����
*****************************************************************************
*/
void InitTimer3(void)
{
	DisableTimer3();
	TIME_Delay(3,10);
}
/*
*****************************************************************************
**�� �� ����void GatherVariable(void) 
**���������� ���ݲɼ���ز�����ʼ��
**��    �룺 
**��    ����
*****************************************************************************
*/
void GatherVariable(void)
{
	GSM_RSTON();                //GPRSΪ��λ״̬
	POWERON();                  //���غɵ�Դ
	G_POWERON();                //�����ٶȴ�������Դ
	SysDat.pADC = 0;	
	SysDat.channelADC = 0xff;
	SysDat.timeLeft = 0;
	SysDat.pFilter = 0;
	MEM_set(GatDat->LoadBuff,0,sizeof(GATHER_DAT));
	if(SysDat.NumRegather == 0)        //�������ݲɼ�
	{
		SysDat.flagGather = 1;
	}
}
/*
*****************************************************************************
**�� �� ����void GatherData(void)
**�������������ݲɼ�
**��    �룺��
**��    ������
*****************************************************************************
*/
void GatherData(void)
{
	//ACCEL_Init();
	if(SysDat.flagGather == 0)     //�����Ҫ���ݲɼ�ֱ���˳�
	{
		return;
	}
	if(ReadMask(SysDat.Flag,SYS_BE_WORKING) == SYS_BE_WORKING)     //��ʼ���ݲɼ�
	{
		ClearMask(SysDat.Flag,SYS_BE_WORKING);
		InitTimer3();
		Timer3IntSwitchOn();	
		GatherVariable();
	}		
	if(SysDat.flagGather == 1)
	{		
		//Timer3Open();		
		while(SysDat.flagGather != 2)             //�ȴ���ѹ���¶����ݲɼ����
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
						ParDat.voltage = (Uint8)(Filter(GatDat->TempLoad,5)>>2);	  //ȡADC��8λ
						ParDat.temperature  = (Uint8)(Filter(GatDat->TempAcc,5)>>2);	  //ȡADC��8λ
						SysDat.flagGather = 2;
						break;	
					}
					SysDat.timeLeft = 10;
					Timer3Open();
				}
			}
			PCON |= 0x01;                              //���з�ʽ��ʡ�磩����Timer3�жϻ���
		}
	}
	if(SysDat.flagGather == 2)
	{
		if(ReadMask(SysDat.Flag,ACC_INT_MASK) != ACC_INT_MASK)     //���ٶ�ģ���Ƿ��ʼ��
		{
			ACCEL_Init();
		}
		//Timer3Open();		
		if(SysDat.pADC<240)                            //�ȴ��غɺͼ��ٶ����ݲɼ����
		{
			if(!SysDat.timeLeft)
			{
				if(!AD0BUSY)
				{
					ADC0_Converx(LOAD);
					if(ReadMask(SysDat.Flag,ACC_INT_MASK) == ACC_INT_MASK)
					{
						MMA8452_Active();//�������ٶȴ�����
						while(ReadMask(SysDat.Flag,ACC_DATA_MASK) != ACC_DATA_MASK); //�ȴ����ٶ�ת����ȡ���					
						ClearMask(SysDat.Flag,ACC_DATA_MASK);
					}
					SysDat.pFilter ++;
					if(SysDat.pFilter>=5)
					{
						SysDat.pFilter = 0;
						GatDat->LoadBuff[SysDat.pADC] = (Uint8)(Filter(GatDat->TempLoad,5)>>2);	  //ȡADC��8λ
						GatDat->AccInt[SysDat.pADC] = Filter(GatDat->TempAcc,5);
						GatDat->AccBuff[SysDat.pADC] = (Uint8)(GatDat->AccInt[SysDat.pADC]>>2);	  //ȡADC��8λ
						SysDat.pADC++;						
					}	
					//ADC0_Converx(ACCELERATION);
					SysDat.timeLeft = ParDat.TimeUnit;         //�²������
					Timer3Open();                              //������ʱ��					
				}
			}
			PCON |= 0x01;			        //���з�ʽ��ʡ�磩����Timer3�жϻ���
		}
		else
		{
			ACCEL_OFF();                      //�رռ��ٶȴ�����			
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
**�������ƣ�void ParaInit(void)                                                                        
**�������ܣ�ϵͳ������ʼ��                                           
**��ڲ����� ��                                                        
**���ز����� ��                                                                 
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
**�� �� ����void InitVariable(void)
**����������������ʼ��
**��    �룺 ��
**��    ���� ��
*****************************************************************************
*/
void InitVariable(void)
{
	ParaInit();
	InitTimer3();
	SysDat.TimerTest = 10;          //�ȴ�10S������ZigBee
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
/*�������ƣ�void SYS_Init(void)                                                    */
/*�������ܣ�ϵͳ��ʼ��                                                             */
/*��ڲ�������                                                                     */
/*���ز�������                                                                     */
/***********************************************************************************/
void SYS_Init(void)
{
	CloseWT();
	//���ݻ���
	GatDat = (GATHER_DATP)(XGDat);
	LinkDat = (LINK_DATP)(XGDat);
	BufDat = (BUF_DATP)(XGDat + 100);
	IOPortInit();                          //IO�ڳ�ʼ��
	SysClkInit(CLK_EXT_OSC,OSC_SYSCLK_Div1,EXT_OSC_CRY,EXT_OSC_FREQUENCY_RANGE7);    //ϵͳʱ�ӳ�ʼ��
	Timer23Init(2,TIMER23_CLKSOURCE_CLK_0);		   //��ʱ��2��ʼ��
	Timer23Init(3,TIMER23_CLKSOURCE_CLK_0);        //��ʱ��3��ʼ��
	UART0Init(UART0_MODE1);                        //Ӳ�����ڳ�ʼ��
	SW_UART_INIT();                                //������ڳ�ʼ��
	SW_UART_ENEABLE();
	SYS_Delay(200);
	GlobleIntSwitchOn();							//ȫ���жϿ�
}
/*
*****************************************************************************
**�� �� ����void Sleep(void)
**����������ϵͳ������ģʽ
**��    �룺 
**��    ����
*****************************************************************************
*/
void Sleep(void)
{
	//Send_Over();
	GlobleIntSwitchOn();				//ȫ���жϿ�
	RTC_Init();                         //��ʼ��SmaRTClock
   	RTC_SetCurrentTime(0);				//����SmaRTClock��ֵ
	RTC_WriteAlarm(ParDat.SpaceTime*ONE_MIN_TICKS);	   //����SmaRTClock�澯ֵ
	if((ReadMask(SysDat.Flag,SYS_HOST_MASK)== SYS_HOST_MASK)&&(ReadMask(SysDat.Flag,ZB_ONLINE_MASK) == ZB_ONLINE_MASK))  //Э����Coordinator
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
**�� �� ����void ConnectGprsNet(void)
**��������������GPRS
**��    �룺 ��
**��    ���� ��
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
**�� �� ����void DetectCommand(void)
**�����������������ķ�������
**��    �룺  ��
**��    ����  ��
*****************************************************************************
*/
void DetectCommand(void)           //�������ķ�������
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
								case 0x01:                //�޸ķ��ͼ��
									if(LinkDat->UDP_Data[i+1])
									{
										//spaceTime = LinkDat->UDP_Data[i+1];
										ParDat.SpaceTime = LinkDat->UDP_Data[i+1];
										SetMask(SysDat.Flag,CAL_CHAN_MASK);
										//SaveToFlash(PARAM_ADDR,0,&UDP_Data[i+1],1);
									}
									break;
								case 0x02:                 //�޸��غ����ݲɼ�ʱ��
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
								case 0x03:                   //�޸ı������ݲɼ���ʽ
									//if(UDP_Data[i+1]<=MAX_TIMEUNIT && UDP_Data[i+1]>0)
									{
										//SaveToFlash(PERIOD_ADDR,0,&UDP_Data[i+1],1);
										//i = 0;
										//SaveToFlash(PARAM_ADDR,1,&i,1);
									}
									break;
								case 0x21:                  //�޸�����IP��4�����˿ںţ�2���������ֲ������У�飨1��
									if(MsgCheckSum(LinkDat->UDP_Data+i+1,6)==LinkDat->UDP_Data[i+7])
									{
										//SaveStringFlash(StartAddress,CENTER_IP_POS,UDP_Data+i+1,6);
										MEM_cpy(ParDat.DeviceIpAddr,&LinkDat->UDP_Data[i+1],4);
										MEM_cpy((Uint8 *)&ParDat.Port,&LinkDat->UDP_Data[i+5],2);
										SetMask(SysDat.Flag,CAL_CHAN_MASK);
										i += 6;
									}
									break;
								case 0x22:                  //���;����޸�ͬ��IP��4�����˿ںţ�2���������ֲ������У�飨1��
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
				ClearMask(SysDat.Flag,ZB_ONLINE_MASK);       //�������־������������
				ZigBeeSend(LinkDat->UDP_Data,LinkDat->UDP_Data_Len);
				SetMask(SysDat.Flag,ZB_ONLINE_MASK);        //������Ϣ�·���ɣ���ZigBee���߱�־
				centerAnswer = 1;
				SysDat.TimeResend = 0;
			}
		}
		//isDisposingCom = 0;
	} 
}
/*
*****************************************************************************
**�� �� ����void SendDataToCenter(void)
**���������� ��������
**��    �룺 ��
**��    ���� ��
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
**�� �� ���� void SendRouterDatToCenter(void)
**���������� �����ӽڵ����ݵ�����������������Ӧ���·� (�˹��̽�ֹ����·���ϴ�����Ϣ)
**��    �룺 
**��    ����
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
			DISK_Read(SysDat.BuffAddr,Msg.Head,DATA_LONG-21);		//���� SIMID[20]��checkSum
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
**�� �� ����void RouterDatProcess(void)
**���������� ·���ϴ����ݴ���
**��    �룺 
**��    ����
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
			SysDat.SecCount = 180;    //3����
			SetMask(SysDat.Flag,SEC_COUNT_MASK);
		} */
		SysDat.SecCount = 0;
	}
	if(ParDat.RecCount >= 5 || (!SysDat.SecCount))       //����5�����ϻ�3����ʱ�䵽���������ϴ�
	{		
		Timer2Off();
		SysDat.SecCount = 180;
		SendRouterDatToCenter();		
		ClearMask(SysDat.Flag,SEC_COUNT_MASK);
		USER_LED = 0;
		POWEROFF();                         //�ر�5V��3.6V��Դ
		Flag_NetOk = 0;
	}
	/*if(ReadMask(SysDat.Flag,SEC_COUNT_MASK) != SEC_COUNT_MASK)
	{
		Timer2Open();
		SysDat.SecCount = 180;    //3����
		SetMask(SysDat.Flag,SEC_COUNT_MASK);
	} */
	
}
/*
*****************************************************************************
** �� �� ��: void SYS_Daemon(void) 
** ��������: ϵͳ����
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
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
		GatherData();            //���ݲɼ� dataReady = 1,SysDat.flagGather = 0
		SysDat.MainLoopError = 0;
		MakeMessage();           //���ݴ��� msgReady = 1,dataReady = 0
	}
	DISK_Write(DISK_OURDATA_ADDR,Msg.Head,DATA_LONG);
	//DISK_Read(DISK_OURDATA_ADDR,Msg.Head,DATA_LONG);
	SysDat.MainLoopError = 0;
	if((ReadMask(SysDat.Flag,SYS_HOST_MASK) == SYS_HOST_MASK) || (ReadMask(SysDat.Flag,ZB_ONLINE_MASK) != ZB_ONLINE_MASK))
	{
		/*DISK_Write(SysDat.PointAddr,Msg.Head,DATA_LONG);
		ParDat.RecCount ++;
		SysDat.PointAddr = SysDat.PointAddr + DATA_LONG;
		if(ReadMask(SysDat.Flag,SEC_COUNT_MASK) != SEC_COUNT_MASK)     //�������ݲɼ�ǰû�յ��ӻ����ݣ��ȴ�3����
		{
			Timer2Open();
			SysDat.SecCount = 180;    //3����
			SetMask(SysDat.Flag,SEC_COUNT_MASK);
		}
		else         //�������ݲɼ�ǰ�յ����ӻ�������������
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
		POWEROFF();                         //�ر�5V��3.6V��Դ
		Flag_NetOk = 0;
		SW_UART_ENEABLE();		
	}
	else 
	{
		CompleteMsg();
		SW_UART_ENEABLE();
		for(SysDat.TimeResend = 0;SysDat.TimeResend < 3;SysDat.TimeResend++)   //��෢��3��ÿ�μ��200��
		{
			ZigBeeSend(Msg.Head,sizeof(MESSAGE_DATA));
			MEM_set(BufDat->Dat,0,sizeof(BUF_DAT));
			SysDat.MainLoopError = 0;
			SysDat.SecCount = 200;    //200��
			SetMask(SysDat.Flag,SEC_COUNT_MASK);
			while((!ZB_Rce_Complete) && (SysDat.SecCount));       //�ȴ�Э��Ӧ��	
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
** �� �� ��: void RTC_ALRM(void) interrupt RTC0_ALARM_PRIOR_ORDER
** ��������: RTC�澯�ж�
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ����  ��: xzy
** �ա�  ��: 2013.
*****************************************************************************
*/ 
//INTERRUPT(RTC_ALRM,INTERRUPT_RTC0ALARM)
void RTC_ALRM(void) interrupt RTC0_ALARM_PRIOR_ORDER
{
	GlobleIntSwitchOff();		//ȫ���жϹ�	
	//USER_LED = ~USER_LED;
	RTC_SetCurrentTime(0);				// 
	RTC_WriteAlarm(ParDat.SpaceTime*ONE_MIN_TICKS);//WAKE_INTERVAL_TICKS);// Set the Alarm Value
	SetMask(SysDat.Flag,SYS_BE_WORKING);
	GlobleIntSwitchOn();		//ȫ���жϿ�
}

















	