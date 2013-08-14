/***********************************************************************************/
/*�ļ�˵���������ļ�                                                               */
/*�ļ����ƣ� user_cfg.h                                                           */
/*�������ڣ�2013.04                                                            */
/*�� �� �ˣ�xzy                                                                   */
/*---------------------------------------------------------------------------------*/
/*�޸����ڣ�                                                                       */
/*�޸�˵����                                                                       */
/***********************************************************************************/
#ifndef USER_CFG_H
#define USER_CFG_H

//---------------------------------------------------------------------------------
//˵����Ԥ������ƺ�����
//---------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//˵��: ���ݳ���
//-------------------------------------------------------------------------------------
#define POINTS_NUM 120		//���ݳ���
#define BUF_NUM	300          //���泤��
#define DATA_LONG 275       //�����������ݳ���
#define MAX_POINT_NUM 187	//���洢��
	              
#define EnableHWRcvCom()       (SCON0 |= 0x10)		 //Ӳ�����ڽ��տ���
#define DisableHWRcvCom()      (SCON0 &= ~0x10)

#define EnableSWRcvCom()       (PCA0CPM0 |= 0x01)		 //	������ڽ��տ���
#define DisableSWRcvCom()      (PCA0CPM0 &= ~0x01)

//---------------------------------------------------------------------------------
//˵�����������
//---------------------------------------------------------------------------------
 // arithmetic  ����
#define	TOTAL				240
#define MIN_POINT			30
#define	MAX_POINT			40
#define MIN_DIF				5
#define QNI					7
#define	ZQWC				5
// message and command  ��Ϣָ��
#define ANSWER_BYTE			9
#define	CENTER_IP_POS		3
#define TARGET_IP_POS		9
//---------------------------------------------------------------------------------
//˵�����ź���
//---------------------------------------------------------------------------------
#define SUCCESS 1
#define ERROR   255
#define TRUER   1
#define FALSE   0
#define NULL    0

#define MEM_INIT_FLAG  0xa0 //�洢����ʼ����־ A0	

// gather channel  �ɼ�ͨ��
/*#define VOLTAGE				0x0A	//��ѹP1.2
#define TEMPERATURE			0x1B	//�¶ȴ����� 
#define LOAD				0x08	//P1.0
#define ACCELERATION		0x09	//���ٶ�P1.1
*/
#define VOLTAGE				0x09	//��ѹP1.1
#define TEMPERATURE			0x1B	//�¶ȴ����� 
#define LOAD				0x08	//P1.0     

#define ACCEOK		0x04	//���ٶ�оƬ���ݾ����˿�ֵ��P1.2��

//---------------------------------------------------------------------------------
//˵����memory��ַ
//---------------------------------------------------------------------------------
#define DISK_PARA_ADDR		0                                         //������ַ
#define DISK_BUFF_ADDR		128                                       //���㻺��
#define DISK_OURDATA_ADDR	(DISK_BUFF_ADDR + TOTAL* QNI*2*4)//13568  //�������ݴ洢��ַ
#define DISK_DATA_SADDR     (DISK_OURDATA_ADDR + DATA_LONG)           //�������ݴ洢��ַ
/*#define DATA_ADDR  20
#define IDATA_ADDR 128
#define XDATA_ADDR 0

#define DISK_PARA_ADDR   0										 //64�ֽڴ洢����
#define DISK_SETUP_ADDR  (DISK_PARA_ADDR + 16 * 4)				 //4�ֽڣ��ߺš���λ����ʱʱ��
#define DISK_MEAS_ADDR   (DISK_PARA_ADDR + 16 * 4 + 4)			 //5�ֽ� �ߵ�š��ܵ�š��ϴα���洢�ܵ��
#define DISK_PEAK_ADDR   (DISK_PARA_ADDR + 16 * 4 + 4 + 5)		 //3�ֽ� ��λ����ֵ��Ѱ���־ ���������
#define DISK_NoNumber_ADDR (DISK_PARA_ADDR + 16 * 4 + 4 + 5 + 3) //�������
//#define DISK_COMM_SETUP_ADDR (DISK_PARA_ADDR + 16 * 4 + 4)   	 //	4�ֽ� ͨ����ʼ��š�ͨ�ż�������

//#define DISK_DATA_BADDR  0
#define DISK_DATA_BADDR  256
#define DISK_DATA_SADDR  (65536 - 128 - 41 * 2)  */



//---------------------------------------------------------------------------------
//˵�����ڴ滺��������
//---------------------------------------------------------------------------------
#define XRAM_SIZE  (0x5C8)



/*//---------------------------------------------------------------------------------
//˵����������־
//--------------------------------------------------------------------------------- 
#define NEGATIVE_SIGN 0xf0 

//---------------------------------------------------------------------------------
//˵����EXTRAM mask
//---------------------------------------------------------------------------------
#define EXTRAM        0x02*/

#define ZB_SN_MODE		0
#define ZB_AT_MODE		1
#define ZB_SPI_MODE		2
//---------------------------------------------------------------------------------
//˵����ϵͳ��־����
//---------------------------------------------------------------------------------
#define SYS_BE_WORKING		0x01             //���ݲɼ�
#define SYS_HOST_MASK		0x02            //���ӻ���־��1��������0���ӻ� ��ZigBeeģ�����
#define ZB_ONLINE_MASK		0x04            //ZigBee���߱�־
#define TIME_ON_MASK		0x08            // ��ʱ��3��
#define ACC_DATA_MASK       0x10			//���ٶ�����������־
#define ACC_INT_MASK		0x20			//���ٶȳ�ʼ����־
#define CAL_CHAN_MASK		0x40			//�����޸ı�־
#define SEC_COUNT_MASK		0x80			//�������־




//---------------------------------------------------------------------------------
//˵����  Set or clear all bits in the mask
//---------------------------------------------------------------------------------

#define ClearMask(addr,mask) addr = (addr & ~(mask))
#define SetMask(addr,mask) addr = (addr | (mask))
#define SeeMask(addr,mask) addr = (addr & mask) 

#define ReadMask(addr,mask) (addr & (mask))
#define WriteMask(addr,data,mask) addr = (addr & ~(mask)) | (data)
#define AntiMask(addr,mask) addr=(addr | mask) & ~(addr & mask)


//---------------------------------------------------------------------------------
//˵�������ſ��ƶ���
//---------------------------------------------------------------------------------
#define	G_POWERON()		G_POWER = 1	   //���ٶȵ�Դ����
#define	G_POWEROFF()	G_POWER = 0

#define POWERON()		SYS_POWER = 1	  //ϵͳ��Դ����
#define POWEROFF()		SYS_POWER = 0

#define ZIGBEE_RSTON()	ZIGBEE_RST = 1 //ZIGBEE��λ����
#define ZIGBEE_RSTOFF()	ZIGBEE_RST = 0 //ZIGBEE��λ����

#define GSM_RSTON()		GSM_REST = 1 //GSM��λ����
#define GSM_RSTOFF()	GSM_REST = 0 //GSM��λ����
//---------------------------------------------------------------------------------
//˵����IIC������ַ
//---------------------------------------------------------------------------------
//#define ADDR_AT24C512A	0xa0
#define ADDR_MMA8452Q	0x38
#define ADDR_FM24V05	0xa0
//---------------------------------------------------------------------------------
//˵��:���ֲ���
//---------------------------------------------------------------------------------



#endif