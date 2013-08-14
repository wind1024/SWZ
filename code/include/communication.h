#ifndef COMMUNICATION_H
#define COMMUNICATION_H
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "user_type.h"
/* =========================== DEFINE AREA ================================= */
#define PPPINITFCS16		0xffff				/* Initial FCS value */
#define PPPGOODFCS16		0x3e62				/* Good final FCS value */
#define ENABLE_UART0()		ES0 = 1
#define DISABLE_UART0()		ES0 = 0
/* =========================== VARIABLE AREA =============================== */
//sbit 	GPRS_LED = P2^4;
//sbit	USER_LED = P2^5;
// -----------------------------------------------------------------------------------------------

/* ========================================================================= */
extern void TestAndConnect(BYTE times);
void DetectData_WaveCom(void);
void WAVE_Rec_Ack(void);
void VariableInit(void);
void Baudrate_Init(void);
void GSM_INI(void);
void InitApn(void);
void RestartGprs(void);
void AT(void);
void ATE(void);
void ATCCID(void);
//void ATE0(void);
//void ATCMGF0(void);
void ATD99(void);
void DEAL_DATA_WAVECOM(void);
// GPRS MESSAGE
void EXCH_SEND(BYTE len);
void EXCH_SEND8(void);
void Fcs16(unsigned char *cp,int len);
WORD pppfcs16(UINT fcs,unsigned char *cp,int len);
extern void SEND_DATA_WAVECOM(BYTE *p,WORD data_len);
void CHECKSUM_UDP(unsigned int Data_length);
long cksum(unsigned char *ip, int len);
////////////////////////////////////////////////////////////////////////////////////////////////////
#endif