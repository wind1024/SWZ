/*
**********************************************************************************
**文件说明： GPRS连接操作                                                           
**文件名称： communication.c                                                  
**创建日期：                                                 
**创 建 人： 来自以前文件                                                                
**---------------------------------------------------------------------------------*
**修改日期： 2013.07.17                                                                   
**修改说明： 对变量进行统一管理，对函数进行整理 ，但总体并未改变                                                                  
************************************************************************************
*/
#include "pin_define.h"
#include "communication.h"
#include "SYS_ABOUT.H"
#include "memory.h"
#include "C8051F930_lib.h"
#include "AT_Commands.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
void TestAndConnect(Uint8 times)
{
	Uint16 xdata i;
	VariableInit();
	InitTimer3();
	while(times)
	{
		DISABLE_UART0();
		GSM_INI();
		if(!Signal_OK)
		{
			times --;
			continue;
		}
		ENABLE_UART0();												// Enable serial port 0	
		//for(i=0;i<1000;i++) Delay10ms();
		for(i=0;i<1000;i++) SYS_Delay(10);	 
		ATD99();      												// Connect commmand : ATD*99***1# 
		//SysDat.Link_Timer = 90;											// Time is 90 seconds		
		SysDat.TimerTest = 90;
		SetMask(SysDat.Flag,TIME_ON_MASK);
		EnableTimer3();
		//while(!Flag_NetOk && SysDat.Link_Timer)
		while(!Flag_NetOk && SysDat.TimerTest)	
		{
			DetectData_WaveCom();
			if(EXCH_SEND4_Flag)	
			{
			 	EXCH_SEND4_Flag = 0;				
				//EXCH_SEND4();
				MEM_cpy(LinkDat->EXCHANGE,EXCHANGE4_CODE,21);
				EXCH_SEND(21);
			}
		}
		if(Flag_NetOk)
		{
			DisableTimer3();
			ClearMask(SysDat.Flag,TIME_ON_MASK);
			times = 0;
			USER_LED = 1;											// connect net ok
		}
		else
		{
			DisableTimer3();
			ClearMask(SysDat.Flag,TIME_ON_MASK);
			times --;
			if(ParDat.csqGPRS == 0)	times = 0;							// Add by scf 2007-3-20 for low power	
			EXCH_SEND8();
			//for(i=0;i<1000;i++) Delay10ms();
			for(i=0;i<1000;i++) SYS_Delay(10);
		}
	}
}
/* ==================================== INIT PRO =========================================== */
void VariableInit(void)         //对GPRS连接需要用到的部分变量和标志初始化
{
	//位变量
	Signal_OK = 0;
	RECEIVE_FLAG = 0;
	PADDING_Flag = 0;
	PPP_Rec_Complete = 0;
	Flag_NetOk = 0;
	PPP_First_Receive = 0;
	WAVE_Data_Receive = 0;
	EXCH_SEND4_Flag = 0;
	WAVE_Data_Receive = 0;
	UDP_Received = 0;
	//
	//SysDat.Link_Timer = 0;
	LinkDat->WAVE_Receive_Num = 0;
	SysDat.ResendTimer = 0;
	DisableTimer3();
	ClearMask(SysDat.Flag,TIME_ON_MASK);
	SysDat.TimerTest = 0;
	

	MEM_cpy(LinkDat->IP_Head,IP_Head_CODE,20);
	MEM_cpy(LinkDat->UDP_Head,UDP_Head_CODE,8);
}
/*  //原文件注释
void SetUart0(WORD baudrate)
{
	if (SYSCLK/baudrate/2/256 < 1)
	{ 
		TH1 = -(SYSCLK/baudrate/2);
      	CKCON &= ~0x0B;                  			// T1M = 1; SCA1:0 = xx
      	CKCON |=  0x08;
	}
   	else if (SYSCLK/baudrate/2/256 < 4) 
	{
      	TH1 = -(SYSCLK/baudrate/2/4);
      	CKCON &= ~0x0B;                  			// T1M = 0; SCA1:0 = 01
      	CKCON |=  0x01;
	}
   	else if (SYSCLK/baudrate/2/256 < 12) 
	{
      	TH1 = -(SYSCLK/baudrate/2/12);
      	CKCON &= ~0x0B;
	}                  								// T1M = 0; SCA1:0 = 00
   	else
	{ 
      	TH1 = -(SYSCLK/baudrate/2/48);
     	CKCON &= ~0x0B;                  			// T1M = 0; SCA1:0 = 10
      	CKCON |=  0x02;
	}
	TL1 = TH1;                          			// Init Timer1
   	TMOD &= ~0xf0;                      			// TMOD: timer 1 in 8-bit autoreload
   	TMOD |=  0x20;
   	TR1 = 1;                            			// START Timer1
   	TI0 = 1;                            			// Indicate TX0 ready
}*/
void InitBaudrate(void)     //原文件注释
{
//	UINT xdata i;

//	RCAP2L = 0xFB;
/*	SetUart0(9600);
	for(i=0;i<sizeof(BaudrateInit);i++)
    {
		TI0 = 0;
		SBUF0 = BaudrateInit[i];
		while(TI0==0); TI0 = 0;
	}
	for(i=0;i<5;i++)
	Delay10ms();

//	RCAP2L = 0xF6;
/*	SetUart0(19200);
	for(i=0;i<sizeof(BaudrateInit);i++)
    {
		TI0 = 0;
		SBUF0 = BaudrateInit[i];
		while(TI0==0); TI0 = 0;
	}
	Delay10ms();

//	RCAP2L = 0xF1;
	SetUart0(38400);
	for(i=0;i<sizeof(BaudrateInit);i++)
    {
		TI0 = 0;
		SBUF0 = BaudrateInit[i];
		while(TI0==0); TI0 = 0;
	}
	Delay10ms();

//	RCAP2L = 0xE2;
	SetUart0(57600);
	for(i=0;i<sizeof(BaudrateInit);i++)
    {
		TI0 = 0;
		SBUF0 = BaudrateInit[i];
		while(TI0==0); TI0 = 0;
	}
	Delay10ms();
		
	SetUart0(115200);
	for(i=0;i<sizeof(BaudrateInit);i++)
    {
		TI0 = 0;
		SBUF0 = BaudrateInit[i];
		while(TI0==0); TI0 = 0;
	}
	for(i=0;i<5;i++)
	Delay10ms();
*/	
//	SetUart0(BAUDRATE);
//	RCAP2L = 0xc4;
//	TR2 = 1;
}
void GSM_INI(void)                //GPRS模块初始化
{
    //Uint8 xdata i;                //BufDat.BitPoint
	//Uint8 xdata Rec[40] = 0;      // BufDat.Dat
	//Uint8 xdata times = 0;        // BufDat.NumCount

	//BufDat->NumCount = 0;
	//memset(Rec,0,sizeof(Rec));
	MEM_set(BufDat->Dat,0,sizeof(BUF_DAT));
    RI0=0;
	Signal_OK = 0;
	SysDat.TimerTest = 0;
	DisableTimer3();
	ClearMask(SysDat.Flag,TIME_ON_MASK);
    while(!Signal_OK)
    {
		if(!SysDat.TimerTest)
		{
			RestartGprs();
			InitBaudrate();											//avoid the random baudrate *scf 2005-6-5*
			AT();
			for(BufDat->BitPoint=0;BufDat->BitPoint<80;BufDat->BitPoint++) SYS_Delay(10);
			AT();
			for(BufDat->BitPoint=0;BufDat->BitPoint<80;BufDat->BitPoint++) SYS_Delay(10);
			ATE();
			for(BufDat->BitPoint=0;BufDat->BitPoint<80;BufDat->BitPoint++) SYS_Delay(10);
			SysDat.TimerTest = 10;											// Use 10s to test the CSQ,I think it is enough.
			BufDat->NumCount ++;
			SetMask(SysDat.Flag,TIME_ON_MASK);
			EnableTimer3();
		}
    	for(BufDat->BitPoint=0;BufDat->BitPoint<7;BufDat->BitPoint++)
    	{
      	  	SBUF0 = GSMCommand1[BufDat->BitPoint];									// AT+CSQ
		 	while(TI0==0); TI0 = 0;
      	  	while(RI0==0)
		  	{
		  		if(!SysDat.TimerTest)
					break;
		 	}
		 	BufDat->Dat[BufDat->BitPoint] = SBUF0;
      	  	RI0 = 0;
    	}

    	for(BufDat->BitPoint=7;BufDat->BitPoint<15;BufDat->BitPoint++)                              				// Error state
    	{
      	  	while(RI0==0)
		  	{
		  		if(!SysDat.TimerTest)
					break;
		  	}
		  	BufDat->Dat[BufDat->BitPoint] = SBUF0;
      	  	RI0 = 0;
	      
		}
	    if(BufDat->Dat[8] != 'R' && BufDat->Dat[9] != 'R' && BufDat->Dat[10] != 'R' && BufDat->Dat[11] != 'R')
	    {
          for(BufDat->BitPoint=15;BufDat->BitPoint<25;BufDat->BitPoint++)
          {  
	        while(RI0==0)
			{
		  		if(!SysDat.TimerTest)
					break;
		 	}
			BufDat->Dat[BufDat->BitPoint] = SBUF0;
      	    RI0 = 0;
      	  }
       	}
        for(BufDat->BitPoint=8;BufDat->BitPoint<25;BufDat->BitPoint++)
		{
			if(BufDat->Dat[BufDat->BitPoint] == ':' && BufDat->Dat[BufDat->BitPoint+2] <= '3'&& BufDat->Dat[BufDat->BitPoint+2] >= '1' && BufDat->Dat[BufDat->BitPoint+3]<='9'&&BufDat->Dat[BufDat->BitPoint+3]>='0' && BufDat->Dat[BufDat->BitPoint+5] !='9')
			{
				Signal_OK = 1;
				ParDat.csqGPRS = (BufDat->Dat[BufDat->BitPoint+2]-'0')*10+(BufDat->Dat[BufDat->BitPoint+3]-'0');
				MEM_set(BufDat->Dat,0,BUF_NUM);
				break;
			}
		    else if(BufDat->Dat[BufDat->BitPoint]==':'&&BufDat->Dat[BufDat->BitPoint+2]>'5'&&BufDat->Dat[BufDat->BitPoint+3]==','&&BufDat->Dat[BufDat->BitPoint+4]!='9')
			{
				Signal_OK = 1;
				ParDat.csqGPRS = BufDat->Dat[BufDat->BitPoint+2]-'0';
				MEM_set(BufDat->Dat,0,BUF_NUM);
				break;
			}                
		}
		if(BufDat->NumCount > 2 && !Signal_OK)
		{
			ParDat.csqGPRS = 0;
			break;
		}
		else
		{
			SYS_Delay(10);
		}			
    }
	if(Signal_OK)          //获取 SIMID
	{
		SysDat.TimerTest = 2;
		//ATCCID();
		for(BufDat->BitPoint=0;BufDat->BitPoint < 8;BufDat->BitPoint++)
		{
			SBUF0 = GSMCommand4[BufDat->BitPoint];									// AT+CSQ
		 	while(TI0==0); TI0 = 0;
      	  	while(RI0==0)
		  	{
		  		if(!SysDat.TimerTest)
					break;
		 	}
		 	BufDat->Dat[BufDat->BitPoint] = SBUF0;
      	  	RI0 = 0;
		}
		for(BufDat->BitPoint = 8;BufDat->BitPoint< 17;BufDat->BitPoint++)                          // Error state
    	{
			while(RI0==0)
		  	{
		  		if(!SysDat.TimerTest)
					break;
		  	}
		  	BufDat->Dat[BufDat->BitPoint] = SBUF0;
      	  	RI0 = 0;
	      
		}
	    if(BufDat->Dat[12] != 'R' && BufDat->Dat[13] != 'R' && BufDat->Dat[15] != 'R')    //不是 ERROR
	    {
          for(BufDat->BitPoint=17;BufDat->BitPoint<47;BufDat->BitPoint++)
          {  
	        while(RI0==0)
			{
		  		if(!SysDat.TimerTest)
					break;
		 	}
			BufDat->Dat[BufDat->BitPoint] = SBUF0;
      	    RI0 = 0;
      	  }
		  for(BufDat->BitPoint=10;BufDat->BitPoint<47;BufDat->BitPoint++)
		  {
			  if(BufDat->Dat[BufDat->BitPoint] == ':')
			  {
				   MEM_cpy(Msg.SIMID,&BufDat->Dat[BufDat->BitPoint+3],20);
				  break;
			  }
		  }
       	}
		else
		{
			 Signal_OK = 0;
		}
	}
	DisableTimer3();
	ClearMask(SysDat.Flag,TIME_ON_MASK);
 	SysDat.TimerTest=0;
	InitApn();
}
void RestartGprs(void)
{
	Uint16 xdata i;
    GSM_REST = 1;
	for(i=0;i<100;i++) SYS_Delay(10);
	GSM_REST = 0;			
	for(i=0;i<5000;i++) SYS_Delay(10);  
}
void InitApn(void)         //原文件注释
{
/*	BYTE xdata ApnBuff[50],len,i;
	memset(ApnBuff,0,50);
	len = sizeof("AT+CGDCONT=1,\"IP\",\"")-1;
	memcpy(ApnBuff,"AT+CGDCONT=1,\"IP\",\"",len);
	memcpy(ApnBuff+len,Apn,lenApn);
	memcpy(ApnBuff+len+lenApn,"\"\r",sizeof("\"\r")-1);
	len = len+lenApn+sizeof("\"\r")-1;
	for(i=0;i<len;i++)
	{
		SBUF0 = ApnBuff[i];
		while(TI0==0); TI0 =0;
	}*/
}
/* ================================= COMMAND TO CONNECT ====================================== */
void AT(void)
{
	//Uint8 xdata i;
	for(BufDat->BitPoint=0;BufDat->BitPoint<3;BufDat->BitPoint++)
	{
		SBUF0 = GSMCommand0[BufDat->BitPoint];
		while(TI0==0); TI0 =0;
	}
}

void ATE(void)
{
	//Uint8 xdata i;
	for(BufDat->BitPoint=0;BufDat->BitPoint<sizeof(GSMCommand3);BufDat->BitPoint++)
	{
		SBUF0 = GSMCommand3[BufDat->BitPoint];
		while(TI0==0); TI0 =0;
	}
}
/*void ATCCID(void)
{
	for(BufDat.BitPoint=0;BufDat.BitPoint<sizeof(GSMCommand4);BufDat.BitPoint++)
	{
		SBUF0 = GSMCommand4[BufDat.BitPoint];
		while(TI0==0); TI0 =0;
	}
} */
/*   //原文件注释
void ATE0(void)
{
	BYTE xdata i;
	for(i=0;i<sizeof(GSMCommand4);i++)
	{
		SBUF0 = GSMCommand4[i];
		while(TI0==0); TI0 =0;
	}
}
void ATCMGF0(void)
{
	BYTE xdata i;
	for(i=0;i<sizeof(GSMCommand5);i++)
	{
		SBUF0 = GSMCommand5[i];
		while(TI0==0); TI0 =0;
	}
}*/
void ATD99(void)
{
	Uint8 xdata i;
    for(i=0;i<12;i++)
   	{
   	  SBUF0 = GSMCommand2[i];
   	  while(TI0==0); TI0=0;
	}        
}
// Send Message To Connect
void EXCH_SEND(Uint8 len)
{
	Uint8 i;
	Fcs16(LinkDat->EXCHANGE,len);
	for(i=0;i<len;i++)
	{
		TI0 = 0;
		SBUF0 = LinkDat->EXCHANGE[i];
		while(TI0==0); TI0 = 0;
	}
}

// Send Message to disconnect net
void EXCH_SEND8(void)
{
	Uint8 xdata i,len=sizeof(EXCHANCE8);         
	
//	Fcs16(EXCHANGE7,len);	
	for(i=0;i<len;i++)
	{
		TI0 = 0;
		SBUF0 = EXCHANCE8[i];
		while(TI0==0); TI0 = 0;
	}
}
//FCS check sum of PPP
WORD pppfcs16(UINT fcs,unsigned char *cp,int len)
{
	while (len--)
        fcs = (fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];
    return (fcs);
}
//FCS check sum of PPP
void Fcs16(unsigned char *cp,int len)
{
    UINT xdata trialfcs;
    cp++;
    trialfcs = pppfcs16( PPPINITFCS16, cp, len-4 );
    trialfcs ^= 0xffff;               // complement
    cp[len-4] = (trialfcs & 0x00ff);    // least significant byte first
    cp[len-3] = ((trialfcs >> 8) & 0x00ff);

}

/* ========================================================================================= */
void DetectData_WaveCom(void)
{
	if(PPP_Rec_Complete) 		   
	{
		PPP_Rec_Complete = 0;
		DEAL_DATA_WAVECOM();
	}
	if(WAVE_Data_Receive)
	{
		WAVE_Data_Receive = 0;
		WAVE_Rec_Ack();
	}
}
void WAVE_Rec_Ack(void)
{
	LinkDat->UDP_Data_Len = LinkDat->WAVE_Receive_Num - 31;
	if(LinkDat->UDP_Data_Len<1 || LinkDat->WAVE_Receive_Num>100)
	{
		return;
	}
	LinkDat->UDP_Data = LinkDat->RECEIVEBUFFER+29;
	UDP_Received = 1;
}
void DEAL_DATA_WAVECOM(void)
{
	UINT xdata i;

	LinkDat->RECEIVEBUFFERPOINT = LinkDat->RECEIVEBUFFER;
	if((*LinkDat->RECEIVEBUFFERPOINT==0xFF)&&(*(LinkDat->RECEIVEBUFFERPOINT+2)==0xC0)&&(*(LinkDat->RECEIVEBUFFERPOINT+3)==0x21)&&(*(LinkDat->RECEIVEBUFFERPOINT+4)==0x01) && (PPP_First_Receive==0))
	{   
		MEM_cpy(LinkDat->EXCHANGE,EXCHANGE1_CODE,22);
		EXCH_SEND(22);
		//EXCH_SEND1();
		PPP_First_Receive = 1;
		ENABLE_UART0();		//ES0=1;
	}
	else if((*LinkDat->RECEIVEBUFFERPOINT==0xFF)&&(*(LinkDat->RECEIVEBUFFERPOINT+2)==0xC0)&&(*(LinkDat->RECEIVEBUFFERPOINT+3)==0x21)&&(*(LinkDat->RECEIVEBUFFERPOINT+4)==0x01))
	{
		PPP_First_Receive = 0;				
		//EXCH_SEND2();
		MEM_cpy(LinkDat->EXCHANGE,EXCHANGE2_CODE,30);
		EXCH_SEND(30);		
		//EXCH_SEND3();
		MEM_cpy(LinkDat->EXCHANGE,EXCHANGE3_CODE,21);
		EXCH_SEND(21);
		SysDat.ResendTimer = 50;								//resend EXCHANGE4 time is 50s
		SetMask(SysDat.Flag,TIME_ON_MASK);
		EnableTimer3();
	} 
	else if((*LinkDat->RECEIVEBUFFERPOINT==0xC0)&&(*(LinkDat->RECEIVEBUFFERPOINT+1)==0x23)&&(*(LinkDat->RECEIVEBUFFERPOINT+2)==0x02))
	{
	    SysDat.ResendTimer = 0;
//		DisableTimer3();
		//EXCH_SEND5();
		MEM_cpy(LinkDat->EXCHANGE,EXCHANGE5_CODE,16);
		EXCH_SEND(16);
		ENABLE_UART0();		//ES0=1;
	}
	else if((*LinkDat->RECEIVEBUFFERPOINT==0x80)&&(*(LinkDat->RECEIVEBUFFERPOINT+1)==0x21)&&(*(LinkDat->RECEIVEBUFFERPOINT+2)==0x01))
	{
		MEM_cpy(LinkDat->EXCHANGE,EXCHANGE6_CODE,16);
		for(i=8;i<=12;i++) LinkDat->EXCHANGE[i+1] = *(LinkDat->RECEIVEBUFFERPOINT+i);
		//EXCH_SEND6(); 
		EXCH_SEND(16);		
		ENABLE_UART0();		ES0=1;
	}
	else if((*LinkDat->RECEIVEBUFFERPOINT==0x80)&&(*(LinkDat->RECEIVEBUFFERPOINT+1)==0x21)&&(*(LinkDat->RECEIVEBUFFERPOINT+2)==0x03))
	{
		MEM_cpy(LinkDat->EXCHANGE,EXCHANGE7_CODE,16);
		for(i=8;i<12;i++) 
		{
			LinkDat->EXCHANGE[i+1] = *(LinkDat->RECEIVEBUFFERPOINT+i); 	// Read new IP
			LinkDat->IP_Head[i+4] = *(LinkDat->RECEIVEBUFFERPOINT+i);
		}
		//EXCH_SEND7();
		EXCH_SEND(16);
		{
			//SysDat.Link_Timer = 0;
			DisableTimer3();
			ClearMask(SysDat.Flag,TIME_ON_MASK);
			SysDat.TimerTest = 0;
			Flag_NetOk=1;
		}
	}
	else if((*LinkDat->RECEIVEBUFFERPOINT==0xFF)&&(*(LinkDat->RECEIVEBUFFERPOINT+2)==0xC0)&&(*(LinkDat->RECEIVEBUFFERPOINT+3)==0x21)&&(*(LinkDat->RECEIVEBUFFERPOINT+4)==0x05))
    {     		    
//		EXCH_SEND8();
		Flag_NetOk = 0;
		ENABLE_UART0();		//ES0=1;		
	}											             
	else if((*LinkDat->RECEIVEBUFFERPOINT==0x21)&&(*(LinkDat->RECEIVEBUFFERPOINT+1)==0x45)&&(*(LinkDat->RECEIVEBUFFERPOINT+10)==0x11))
	{
		for(i=0;i<4;i++)
			LinkDat->IP_Head[16+i] = *(LinkDat->RECEIVEBUFFERPOINT+13+i);
		for(i=0;i<2;i++)
			LinkDat->UDP_Head[2+i] = *(LinkDat->RECEIVEBUFFERPOINT+21+i);
		WAVE_Data_Receive = 1;
    }
}
void SEND_DATA_WAVECOM(BYTE *p,WORD data_len)
{
	unsigned int xdata i;
	//unsigned char xdata Temp_Buffer[400];			//rewrited by scf -03-11-07-
	unsigned int xdata buffer_len=0;
	
	if(!Flag_NetOk)	return;

	/*memcpy(IP_Head+16,(BYTE*)(ipPort.ipAddr),4);
	memcpy(UDP_Head+2,(BYTE*)(&(ipPort.port)),2);*/
	
	MEM_cpy(LinkDat->IP_Head+16,(BYTE*)(ParDat.DeviceIpAddr),4);
	MEM_cpy(LinkDat->UDP_Head+2,(BYTE*)(&(ParDat.Port)),2);
	if(p != NULL)
	{
		for(i=0;i<data_len;i++)
			LinkDat->WAVE_Command_Buf[i] = *(p+i);
	}
	// 如为奇数后面补0
	i = data_len%2;
	if(i!=0)
	{
		LinkDat->WAVE_Command_Buf[data_len] = 0;
		data_len++;
	}

	*( unsigned int xdata *)(LinkDat->IP_Head+2)=28+data_len;	// write total length to IP head 
	LinkDat->IP_Head[10]=LinkDat->IP_Head[11]=0x00;						// clear check_sum buffer in IP_HEAD

	*(unsigned int xdata *)(LinkDat->UDP_Head+4)=8+data_len;		// write UDP length to UDP head
	LinkDat->UDP_Head[6] = LinkDat->UDP_Head[7]=0x00;						// clear check_sum buffer in UDP_HEAD

	//Frame_End[0]=Frame_End[1]=0x00;						// clear FCS buffer 
	CHECKSUM_UDP(data_len);

	//for(i=0;i<2;i++)  Temp_Buffer[buffer_len++] = Frame_Head[i];
	LinkDat->SEND_Buffer[buffer_len++] = 0x7e;          //数据帧头
	LinkDat->SEND_Buffer[buffer_len++] = 0x21;
	for(i=0;i<20;i++) LinkDat->SEND_Buffer[buffer_len++] = LinkDat->IP_Head[i];
	for(i=0;i<8;i++)  LinkDat->SEND_Buffer[buffer_len++] = LinkDat->UDP_Head[i];
	for(i=0;i<data_len;i++)	LinkDat->SEND_Buffer[buffer_len++] = LinkDat->WAVE_Command_Buf[i];
	//for(i=0;i<3;i++)  Temp_Buffer[buffer_len++] = Frame_End[i];
	LinkDat->SEND_Buffer[buffer_len++] = 0x00;     // clear FCS buffer
	LinkDat->SEND_Buffer[buffer_len++] = 0x00;
	LinkDat->SEND_Buffer[buffer_len++] = 0x7e;    //数据帧结束

	Fcs16(LinkDat->SEND_Buffer,buffer_len);						// create FSC and write to buffer
	
	for(i=0;i<11;i++)									// send data to UART0
	{
		TI0 = 0;
       	SBUF0 = LinkDat->SEND_Buffer[i];
       	while(TI0==0); TI0 = 0;
	}
	
	SBUF0 = 0x7D;
	while(TI0==0);TI0=0;
	SBUF0 = 0x31;
	while(TI0==0);TI0=0;
	for(i=12;i<buffer_len-1;i++)
	{
		if(LinkDat->SEND_Buffer[i] == 0x7E){
			SBUF0 = 0x7D;
			while(TI0==0);TI0=0;
			SBUF0 = 0x5E;
			while(TI0==0);TI0=0;
		}
		else if(LinkDat->SEND_Buffer[i] == 0x7D){
			SBUF0 = 0x7D;
			while(TI0==0);TI0=0;
			SBUF0 = 0x5D;
			while(TI0==0);TI0=0;
		}
	/*	else if(Temp_Buffer[i]< 0x20){			
			SBUF0 = 0x7D;
			while(TI0==0);TI0=0;
			SBUF0 = Temp_Buffer[i]^0x20;
			while(TI0==0);TI0=0;
		}*/
		else{
			SBUF0 = LinkDat->SEND_Buffer[i];
			while(TI0==0);TI0=0;
		}
	}

	SBUF0 = 0x7E;
	while(TI0==0);TI0=0;
	
}
void CHECKSUM_UDP(unsigned int Data_length)
{
	unsigned int xdata i;
	long xdata sum;
	//unsigned char xdata Temp_Buffer[350];

	sum = cksum(LinkDat->IP_Head,20);
    LinkDat->IP_Head[10] = ((sum >> 8) & 0x00ff);
    LinkDat->IP_Head[11] = (sum & 0x00ff);             		// least significant byte first

    for(i=0;i<8;i++) LinkDat->UDP_Buffer[i] = LinkDat->IP_Head[i+12];

    LinkDat->UDP_Buffer[8] = 0x0;
    LinkDat->UDP_Buffer[9] = LinkDat->IP_Head[9];
    LinkDat->UDP_Buffer[10] = LinkDat->UDP_Head[4];
    LinkDat->UDP_Buffer[11] = LinkDat->UDP_Head[5];

    for(i=0;i<8;i++) LinkDat->UDP_Buffer[i+12] = LinkDat->UDP_Head[i];
	for(i=0;i<Data_length;i++)LinkDat->UDP_Buffer[i+20] = LinkDat->WAVE_Command_Buf[i];
    sum = cksum(LinkDat->UDP_Buffer,20+Data_length);
    LinkDat->UDP_Head[6] = ((sum >> 8) & 0x00ff);
    LinkDat->UDP_Head[7] = (sum & 0x00ff);             // least significant byte first

}

//-------------------------------------------------------------------------------------------------------
long cksum(unsigned char *ip, int len)
{

       register long xdata sum = 0;

        while( len > 1 )
        {
               sum += *((unsigned int *)ip)++;
               len -= 2;
       	}

       if( len > 0 )
               sum += *(unsigned char *)ip;

       while (sum>>16)
           sum = (sum & 0xffff) + (sum >> 16);

       return ~sum;
}
