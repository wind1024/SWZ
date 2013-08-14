/***********************************************************************************/
/*文件说明：配置文件                                                               */
/*文件名称： user_cfg.h                                                           */
/*创建日期：2013.04                                                            */
/*创 建 人：xzy                                                                   */
/*---------------------------------------------------------------------------------*/
/*修改日期：                                                                       */
/*修改说明：                                                                       */
/***********************************************************************************/
#ifndef USER_CFG_H
#define USER_CFG_H

//---------------------------------------------------------------------------------
//说明：预编译控制宏命令
//---------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//说明: 数据长度
//-------------------------------------------------------------------------------------
#define POINTS_NUM 120		//数据长度
#define BUF_NUM	300          //缓存长度
#define DATA_LONG 275       //单组完整数据长度
#define MAX_POINT_NUM 187	//最大存储量
	              
#define EnableHWRcvCom()       (SCON0 |= 0x10)		 //硬件串口接收控制
#define DisableHWRcvCom()      (SCON0 &= ~0x10)

#define EnableSWRcvCom()       (PCA0CPM0 |= 0x01)		 //	软件串口接收控制
#define DisableSWRcvCom()      (PCA0CPM0 &= ~0x01)

//---------------------------------------------------------------------------------
//说明：参数相关
//---------------------------------------------------------------------------------
 // arithmetic  计算
#define	TOTAL				240
#define MIN_POINT			30
#define	MAX_POINT			40
#define MIN_DIF				5
#define QNI					7
#define	ZQWC				5
// message and command  信息指令
#define ANSWER_BYTE			9
#define	CENTER_IP_POS		3
#define TARGET_IP_POS		9
//---------------------------------------------------------------------------------
//说明：信号量
//---------------------------------------------------------------------------------
#define SUCCESS 1
#define ERROR   255
#define TRUER   1
#define FALSE   0
#define NULL    0

#define MEM_INIT_FLAG  0xa0 //存储器初始化标志 A0	

// gather channel  采集通道
/*#define VOLTAGE				0x0A	//电压P1.2
#define TEMPERATURE			0x1B	//温度传感器 
#define LOAD				0x08	//P1.0
#define ACCELERATION		0x09	//加速度P1.1
*/
#define VOLTAGE				0x09	//电压P1.1
#define TEMPERATURE			0x1B	//温度传感器 
#define LOAD				0x08	//P1.0     

#define ACCEOK		0x04	//加速度芯片数据就绪端口值（P1.2）

//---------------------------------------------------------------------------------
//说明：memory地址
//---------------------------------------------------------------------------------
#define DISK_PARA_ADDR		0                                         //参数地址
#define DISK_BUFF_ADDR		128                                       //运算缓存
#define DISK_OURDATA_ADDR	(DISK_BUFF_ADDR + TOTAL* QNI*2*4)//13568  //本身数据存储地址
#define DISK_DATA_SADDR     (DISK_OURDATA_ADDR + DATA_LONG)           //接收数据存储地址
/*#define DATA_ADDR  20
#define IDATA_ADDR 128
#define XDATA_ADDR 0

#define DISK_PARA_ADDR   0										 //64字节存储参数
#define DISK_SETUP_ADDR  (DISK_PARA_ADDR + 16 * 4)				 //4字节：线号、单位、定时时间
#define DISK_MEAS_ADDR   (DISK_PARA_ADDR + 16 * 4 + 4)			 //5字节 线点号、总点号、上次保存存储总点号
#define DISK_PEAK_ADDR   (DISK_PARA_ADDR + 16 * 4 + 4 + 5)		 //3字节 电位器峰值、寻峰标志 、仪器编号
#define DISK_NoNumber_ADDR (DISK_PARA_ADDR + 16 * 4 + 4 + 5 + 3) //仪器编号
//#define DISK_COMM_SETUP_ADDR (DISK_PARA_ADDR + 16 * 4 + 4)   	 //	4字节 通信起始点号、通信计数个数

//#define DISK_DATA_BADDR  0
#define DISK_DATA_BADDR  256
#define DISK_DATA_SADDR  (65536 - 128 - 41 * 2)  */



//---------------------------------------------------------------------------------
//说明：内存缓存区长度
//---------------------------------------------------------------------------------
#define XRAM_SIZE  (0x5C8)



/*//---------------------------------------------------------------------------------
//说明：负数标志
//--------------------------------------------------------------------------------- 
#define NEGATIVE_SIGN 0xf0 

//---------------------------------------------------------------------------------
//说明：EXTRAM mask
//---------------------------------------------------------------------------------
#define EXTRAM        0x02*/

#define ZB_SN_MODE		0
#define ZB_AT_MODE		1
#define ZB_SPI_MODE		2
//---------------------------------------------------------------------------------
//说明：系统标志掩码
//---------------------------------------------------------------------------------
#define SYS_BE_WORKING		0x01             //数据采集
#define SYS_HOST_MASK		0x02            //主从机标志，1：主机；0：从机 由ZigBee模块决定
#define ZB_ONLINE_MASK		0x04            //ZigBee在线标志
#define TIME_ON_MASK		0x08            // 定时器3用
#define ACC_DATA_MASK       0x10			//加速度数据正常标志
#define ACC_INT_MASK		0x20			//加速度初始化标志
#define CAL_CHAN_MASK		0x40			//参数修改标志
#define SEC_COUNT_MASK		0x80			//秒计数标志




//---------------------------------------------------------------------------------
//说明：  Set or clear all bits in the mask
//---------------------------------------------------------------------------------

#define ClearMask(addr,mask) addr = (addr & ~(mask))
#define SetMask(addr,mask) addr = (addr | (mask))
#define SeeMask(addr,mask) addr = (addr & mask) 

#define ReadMask(addr,mask) (addr & (mask))
#define WriteMask(addr,data,mask) addr = (addr & ~(mask)) | (data)
#define AntiMask(addr,mask) addr=(addr | mask) & ~(addr & mask)


//---------------------------------------------------------------------------------
//说明：引脚控制定义
//---------------------------------------------------------------------------------
#define	G_POWERON()		G_POWER = 1	   //加速度电源控制
#define	G_POWEROFF()	G_POWER = 0

#define POWERON()		SYS_POWER = 1	  //系统电源控制
#define POWEROFF()		SYS_POWER = 0

#define ZIGBEE_RSTON()	ZIGBEE_RST = 1 //ZIGBEE复位控制
#define ZIGBEE_RSTOFF()	ZIGBEE_RST = 0 //ZIGBEE复位控制

#define GSM_RSTON()		GSM_REST = 1 //GSM复位控制
#define GSM_RSTOFF()	GSM_REST = 0 //GSM复位控制
//---------------------------------------------------------------------------------
//说明：IIC器件地址
//---------------------------------------------------------------------------------
//#define ADDR_AT24C512A	0xa0
#define ADDR_MMA8452Q	0x38
#define ADDR_FM24V05	0xa0
//---------------------------------------------------------------------------------
//说明:部分操作
//---------------------------------------------------------------------------------



#endif