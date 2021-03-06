/*
**********************************************************************************
**文件说明：软件模拟串口文件                                                           
**文件名称：SWSerial.c                                                    
**创建日期：2013.05.07                                                         
**创 建 人：XZY                                                                   
**---------------------------------------------------------------------------------*
**修改日期：                                                                       
**修改说明：                                                                       
**********************************************************************************
*/
// 软件 UART 程序 使用 PCA 作为波特率发生器  
// PCA 模块 0 用作接收波特率源和起始位检测器,为了检测起始位模块 0 被配置为 
// 负沿捕捉方式,对于所有其它的SW_UART 操作模块 0 被配置为软件定时器,模块 
// 匹配中断用于产生波特率.模块 1 用软件定时器方式产生发送波特率  
// 程序假设一个外部晶体连接在 XTAL1 和 XTAL2 引脚之间  
// 外部晶体的频率应在 SYSCLK 常数中定义  
// 波特率可设置、起始位：1位、数据位：8位、停止位：1位、校验：无校验  
// 初始化过程: 
// 1) 根据外部晶体频率定义 SYSCLK  
// 2) 定义 BAUD_RATE  
// 3) 调用 SW_UART_INIT()  
// 4) 置位 SREN 以允许软件 UART 接收器  
// 5) 置位 SES 只在需要用户级中断支持时  
// 6) 调用 SW_UART_ENABLE()  
// 
// 发送: 
// 1) 查询 STXBSY 看是否为 0  
// 2) 写数据到 TDR  
// 3) 置位 CCF1 以启动发送  
// 4) 发送完成时置位 STI 如果用户级中断被允许 产生一个EIE1.6 比较器1中断  
//
// 接收: 
// 1) 如果是在查询方式 查询 SRI 如果是在中断方式 在EIE1.6 比较器1中断服务程序中查询 SRI
// 2) 从 RDR 读数据  
//  
 
#include "SWSerial.h"
#include "pin_define.h"

bit SRI;                           //接收完成标志
bit STI;                           //发送完成标志
bit STXBSY;                        //发送忙标志
bit SREN;                          //接收允许
bit	SES;						   //SW_UART 用户级中断使能位
 
//sbit SW_RX = P1^4;                   //接收引脚
//sbit SW_TX = P1^5;                   //发送引脚
 
char TDR;                          //发送数据寄存器
char RDR;                          //接收数据寄存器

bit SW_DONE;                       //SW发送结束标志（发送完15个字符）

//测试变量
//char k,m;                          //测试计数器
//char idata SW_BUF[15];             //测试接收缓冲区
 
void PCA_ISR();                     //SW_UART中断服务程序 

/******************************************************************************
** 函 数 名: void SW_UART_INIT(void)
** 功能描述: 软件串口初始化
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.05
******************************************************************************/ 
void SW_UART_INIT(void)
{
	PCA0CPM0 = 0x10;            //模块0为负沿捕捉方式，禁止模块0中断										
	PCA0CPM1 = 0x48;            //模块1为软件定时器方式，禁止模块1中断
	
	PCA0CN = 0;                 //PCA保持禁止状态              
	PCA0MD = 0x02;              //PCA时基=SYSCLK/4;禁止PCA计数器中断
	
	CCF0 = 0;                   //清除PCA模块0和1捕捉比较模块
	CCF1 = 0; 
	                
	SRI = 0;                   //清楚接受完成标志
	STI = 0;                    //清除发送完成标志
	
	SW_TX = 1;                  //将TX线初始化为高电平
	STXBSY = 0;                 //清除SW_UART忙标志
}
//----------------------------------------------------------------------------------
/******************************************************************************
** 函 数 名: void SW_UART_ENEABLE(void)
** 功能描述: 软件串口使能
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
******************************************************************************/ 
void SW_UART_ENEABLE(void)
{ 
	PCA0CPM0 |= 0x01;           //允许PCA模块0（接收中断）
	PCA0CPM1 |= 0x01;           //允许PCA模块1（发送中断）	
	CR = 1;                    //启动PCA计数器
	PCA0IntSwitchOn();        //允许PCA中断
	//GlobleIntSwitchOn();       //全局中断允许
	CPT1MD = 0x10;				//比较器 1 下降沿中断允许 
	SREN = 1;					// 使能数据接收
	SES = 1;					// 用户级中断使能
}
/******************************************************************************
** 函 数 名: void SW_UART_DISABLE(void)
** 功能描述: 关闭软件串口
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
******************************************************************************/ 
void SW_UART_DISABLE(void)
{ 
	PCA0CPM0 &= (~0x01);           //禁用PCA模块0（接收中断）
	PCA0CPM1 &= (~0x01);           //禁用PCA模块1（发送中断）	
	CR = 0;                    //禁用PCA计数器
	PCA0IntSwitchOff();        //禁用PCA中断
	//GlobleIntSwitchOn();       //全局中断允许
	CPT1MD &= (~0x10);				//比较器 1 下降沿中断允许 
	SREN = 0;					// 禁用数据接收
	SES = 0;					// 用户级中断使禁止
}
/******************************************************************************
** 函 数 名: void StartSend(void)
** 功能描述: 启动软件串口发送
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 作　  者: xzy
** 日　  期: 2013.
******************************************************************************/ 
void StartSend(void)
{
	BufDat->NumCount = 0;
	STI=1;
	SW_DONE = 0;
	CP1IntSwitchOn();   // 允许比较器1（CP1）中断
	CPT1CN |= 0x10;    // 触发比较器 1 下降沿中断
}
//-------------------------------------------------------------------------------------
//中断服务程序
//--------------------------------------------------------------------------------------
void PCA_ISR(void) interrupt PCA0_PRIOR_ORDER			 //11
 {
  static char SUTXST = 0;           //SW_UART TX状态变量
  static char SURXST = 0;           //SW_UART RX状态变量
  static unsigned char RXSHIFT;   //SW_UART RX移位寄存器
  static int PCA_TEMP;            //临时储存变量，用于处理PCA模块的高和低字节

//首先检查接受中断变量，如果CCF0置位则对其服务 
  if(CCF0){
     CCF0=0;                      //清除中断标志
     switch(SURXST){
			//状态：0 收到起始位
			//在该状态，是SW_TX上的负边沿触发中断，表示检测到起始位，同时PCA0CP0寄存器捕捉PCA0的值
			//检查接受允许和起始位
			//将PCA模块0切换到软件定时方式
			//加3/2位时间到模块0捕捉寄存器以采样LSB
			//TX状态变量�1
            case 0:
                 if(SREN & ~SW_RX){  //检查接收允许和起始位

                    PCA_TEMP = (PCA0CPH0<<8);//将模块0的内容读到PCA_TEMP
                    PCA_TEMP |= PCA0CPL0;
                    PCA_TEMP += TH_TIME_COUNT;//加3/2位时间到PCA_TEMP

                    PCA0CPL0 = PCA_TEMP;     //更新PCA0CPL0和PCA0CPH0
                    PCA0CPH0 = (PCA_TEMP>>8);

                    PCA0CPM0=0x49;         //将模块0切换到软件定时器方式，允许中断

                    SURXST++;              //更新RX状态变量  
					}              
                    break;
			//状态1-8：收到数据位
			//采样SW_RX引脚
			//将新数据移入RXSHIFT
			//加1个位时间变量到模块0捕捉寄存器
			//RX状态变量�1
             case 1:             
             case 2:
             case 3:              
             case 4:            
             case 5:
             case 6: 
             case 7:
             case 8:
                 RXSHIFT = RXSHIFT >> 1; //右移一位
                 if(SW_RX)             //如果SW_RX=1；将1移入RXSHIFT的MSB
                    RXSHIFT |= 0x80;
                   
                    PCA_TEMP = (PCA0CPH0<<8);//将模块0内容读到PCA_TEMP
                    PCA_TEMP |= PCA0CPL0;

                    PCA_TEMP += TIME_COUNT;//加1个位时间到PCA_TEMP

                    PCA0CPL0 = PCA_TEMP;   //更新PCA0CPL0和PCA0CPH0
                    PCA0CPH0 = (PCA_TEMP>>8);

                    SURXST++;              //更新RX状态变量
                    break;
			//状态9：已收到8个数据位，捕捉停止位
			//将RXSHIFT传到RDR
			//置位SRI（表示接收完成）
			//设置模块0，为下一次传输做准备
			//复位RX状态变量
            case 9:
                 RDR = RXSHIFT;
                 SRI = 1;
                 PCA0CPM0 = 0x11;
                 SURXST = 0;
				 if (SES){      //如果用户级中断支持被允许 
					CP1IntSwitchOn();   // 允许比较器1（CP1）中断
					CPT1CN |= 0x10;    // 触发比较器 1 下降沿中断
				}
                 break;
             }
 }             
//检查发送中断，如果CCF1置位则对其服务
     else if(CCF1){
          CCF1=0;              //清除中断标志
          switch(SUTXST){
		//状态0：发送过程以启动
		//在此，用户已将要发送的字节装入到TDR，强制模块1中断以启动发送
		//发送起始位（使SW_TX变低电平）
		//读PCA0,加一个位时间后存到模块1捕捉寄存器
		//TX状态变量加1
          case 0:
               SW_TX = 0;				  // 使 TX 引脚变低作为起始位 
               PCA_TEMP = PCA0L;		  // 将 PCA 计数器的值读到 PCA_TEMP  
               PCA_TEMP |= (PCA0H<<8);
               PCA_TEMP += TIME_COUNT;	  // 加一个位时间  
               PCA0CPL1 = PCA_TEMP;;	  // 将更新后的匹配值存到 
               PCA0CPH1 = (PCA_TEMP>>8);  // 模块 1 捕捉比较寄存器
               PCA0CPM1 |= 0x48;		  // 允许模块 1 软件定时器
               SUTXST++;				  // 更新 TX 状态变量 
               break;
		//状态1-9：发送数据位
		//将TDR的LSB输入到TX
		//将TDR右移一位
		//将一个1移入TDR的MSB作为状态9的停止位
		//加一个时间到模块1捕捉寄存器
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 9:
               SW_TX = (TDR & 0x01);   // 将TDR的LSB输出到SW_TX引脚  
               TDR >>= 1;			   // TDR 右移一位  
               TDR |= 0x80;			   // 将一个'1'移入TDR的MSB作为状态9的停止位 
               PCA_TEMP = (PCA0CPH1<<8);  // 将模块 0 内容读到PCA_TEMP  
               PCA_TEMP |= PCA0CPL1;
               PCA_TEMP += TIME_COUNT; // 加一个位时间到PCA_TEMP 
               PCA0CPL1 = PCA_TEMP;;   // 用新值恢复 PCA0CPL1和PCA0CPH1 
               PCA0CPH1 = (PCA_TEMP>>8);
    
               SUTXST++;				 // 更新 TX 状态变量
               break;
		//状态10：最后一位数据已发送完。发送停止位并结束传输过程
		//发送停止位
		//置1发送结束标志，清除忙标志
		//复位TX状态
		//设置模块1，为下一次传输做准备
        case 10:
              STI = 1;					 // 表示发送完成
              SUTXST = 0;				 // 复位 TX 状态  
              SW_TX = 1;				 // SW_TX 应保持高电平  
              PCA0CPM1 = 0x01;			 // 禁止模块 1 软件定时器,保持中断为允许状态以备下一次传输
              if (SES){      			//如果用户级中断支持被允许 
				CP1IntSwitchOn();   	// 允许比较器1（CP1）中断
				CPT1CN |= 0x10;    		// 触发比较器 1 下降沿中断  
				}
			  	STXBSY = 0;		   		// SW_UART TX 空闲
			  break;
            }
        }
   
}
//-------------------------------------------------------------------- 
// USER_ISR: 将比较器1中断用作用户 SW_UART 中断服务程序(EIE1.6 比较器1中断) 
// 如果中断测试方式被允许 该 ISR 将发送 15 个字符并接收 15 个字符  
// 每次 SW_UART 发送或接收完成都要触发该例程  
// - 检查接收完成指示标志并服务  
// - 检查发送完成指示标志并服务  
// - 检查 ISR 执行期间是否有发送和接收过程完成 如有 再触发一次中断  
// 
void USER_ISR(void) interrupt COMPARATOR1_PRIOR_ORDER 	  // 13 中断服务程序
{     
	CPT1CN &= ~(0x10);       // 清比较器1下降沿中断标志 
	if (SRI){         // 如果接收完成: 
		SRI = 0;         // 清除接收标志  		
		//BufDat->NumCount ++;						 //接收计数
		if (BufDat->NumCount >= BUF_NUM){        // 如果接收满 
			BufDat->NumCount = 0;
			//BufDat->BitPoint = 0;
			//SREN=0;        // 禁止 SW_UART 接收器 
		}
		BufDat->Dat[BufDat->NumCount] = RDR;      // 读接收缓冲器 
		if((ReadMask(SysDat.Flag,ZB_ONLINE_MASK)) && (ReadMask(SysDat.Flag,SYS_HOST_MASK)))     // ZB_ONLINE_MASK AND  SYS_HOST_MASK
		{
			if((BufDat->NumCount >= 2) && (BufDat->NumCount  < DATA_LONG) && ZB_Rce_Flag )
			{
				BufDat->NumCount ++;
				if(BufDat->NumCount == DATA_LONG)
				{
					ZB_Rce_Complete = 1;
					ZB_Rce_Flag = 0;					
				}
				return;
			}
			switch(BufDat->NumCount)
				{
					case 0 :
						if(BufDat->Dat[BufDat->NumCount] == '*')
						{
							BufDat->NumCount ++;
						}
						else
						{
							BufDat->NumCount = 0;
						}
						break;
					case 1:
						if(BufDat->Dat[BufDat->NumCount] == '*')
						{
							BufDat->NumCount ++;
							ZB_Rce_Flag = 1;
						}
						else
						{
							BufDat->NumCount = 0;
						}
						break;
					default:
						break;
				}
				
		}
		else if((ReadMask(SysDat.Flag,ZB_ONLINE_MASK)) && (!ReadMask(SysDat.Flag,SYS_HOST_MASK)))
		{
			if((BufDat->NumCount >= 2) && ZB_Rce_Flag )
			{
				BufDat->NumCount ++;
				if(BufDat->NumCount == BufDat->Dat[2])
				{
					ZB_Rce_Complete = 1;
					ZB_Rce_Flag = 0;
				}
				return;
			}
			switch(BufDat->NumCount)
				{
					case 0 :
						if(BufDat->Dat[BufDat->NumCount] == '$')
						{
							BufDat->NumCount ++;
						}
						else
						{
							BufDat->NumCount = 0;
						}
						break;
					case 1:
						if(BufDat->Dat[BufDat->NumCount] == '$')
						{
							BufDat->NumCount ++;
							ZB_Rce_Flag = 1;
						}
						else
						{
							BufDat->NumCount = 0;
						}
						break;
					default:
						break;
				}
			
		}
		else
		{
			BufDat->NumCount ++;						 //接收计数
		}
				          
	}  
	else if (STI){        // 如果发送完成: 
		STI = 0;         // 清除发送标志  
		if (BufDat->NumCount < BufDat->BitPoint){             // 如果字符未发送完: 
			STXBSY = 1;                       // 申请 SW_UART 发送器  
			TDR = BufDat->Dat[BufDat->NumCount++];            // 变量加 1 发送
			CCF1 = 1;         // 强制模块 1 中断以启动发送 
		} 
		else
		{ 
			BufDat->NumCount = 0;
			BufDat->BitPoint = 0;
			SREN = 1;					// 使能数据接收
			SW_DONE=1;              // 表示已发送完最后一个字符
		}  
	}  
//	if (STI|SRI)         // 如果 SRI 或 STI 置位 再次触发 
//	CPT1CN |= 0x10;    		// 触发比较器 1 下降沿中断 
}
