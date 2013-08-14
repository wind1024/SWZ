/*
**********************************************************************************
**ÎÄ¼þËµÃ÷£ºÈí¼þÄ£Äâ´®¿ÚÎÄ¼þ                                                           
**ÎÄ¼þÃû³Æ£ºSWSerial.c                                                    
**´´½¨ÈÕÆÚ£º2013.05.07                                                         
**´´ ½¨ ÈË£ºXZY                                                                   
**---------------------------------------------------------------------------------*
**ÐÞ¸ÄÈÕÆÚ£º                                                                       
**ÐÞ¸ÄËµÃ÷£º                                                                       
**********************************************************************************
*/
// Èí¼þ UART ³ÌÐò Ê¹ÓÃ PCA ×÷Îª²¨ÌØÂÊ·¢ÉúÆ÷  
// PCA Ä£¿é 0 ÓÃ×÷½ÓÊÕ²¨ÌØÂÊÔ´ºÍÆðÊ¼Î»¼ì²âÆ÷,ÎªÁË¼ì²âÆðÊ¼Î»Ä£¿é 0 ±»ÅäÖÃÎª 
// ¸ºÑØ²¶×½·½Ê½,¶ÔÓÚËùÓÐÆäËüµÄSW_UART ²Ù×÷Ä£¿é 0 ±»ÅäÖÃÎªÈí¼þ¶¨Ê±Æ÷,Ä£¿é 
// Æ¥ÅäÖÐ¶ÏÓÃÓÚ²úÉú²¨ÌØÂÊ.Ä£¿é 1 ÓÃÈí¼þ¶¨Ê±Æ÷·½Ê½²úÉú·¢ËÍ²¨ÌØÂÊ  
// ³ÌÐò¼ÙÉèÒ»¸öÍâ²¿¾§ÌåÁ¬½ÓÔÚ XTAL1 ºÍ XTAL2 Òý½ÅÖ®¼ä  
// Íâ²¿¾§ÌåµÄÆµÂÊÓ¦ÔÚ SYSCLK ³£ÊýÖÐ¶¨Òå  
// ²¨ÌØÂÊ¿ÉÉèÖÃ¡¢ÆðÊ¼Î»£º1Î»¡¢Êý¾ÝÎ»£º8Î»¡¢Í£Ö¹Î»£º1Î»¡¢Ð£Ñé£ºÎÞÐ£Ñé  
// ³õÊ¼»¯¹ý³Ì: 
// 1) ¸ù¾ÝÍâ²¿¾§ÌåÆµÂÊ¶¨Òå SYSCLK  
// 2) ¶¨Òå BAUD_RATE  
// 3) µ÷ÓÃ SW_UART_INIT()  
// 4) ÖÃÎ» SREN ÒÔÔÊÐíÈí¼þ UART ½ÓÊÕÆ÷  
// 5) ÖÃÎ» SES Ö»ÔÚÐèÒªÓÃ»§¼¶ÖÐ¶ÏÖ§³ÖÊ±  
// 6) µ÷ÓÃ SW_UART_ENABLE()  
// 
// ·¢ËÍ: 
// 1) ²éÑ¯ STXBSY ¿´ÊÇ·ñÎª 0  
// 2) Ð´Êý¾Ýµ½ TDR  
// 3) ÖÃÎ» CCF1 ÒÔÆô¶¯·¢ËÍ  
// 4) ·¢ËÍÍê³ÉÊ±ÖÃÎ» STI Èç¹ûÓÃ»§¼¶ÖÐ¶Ï±»ÔÊÐí ²úÉúÒ»¸öEIE1.6 ±È½ÏÆ÷1ÖÐ¶Ï  
//
// ½ÓÊÕ: 
// 1) Èç¹ûÊÇÔÚ²éÑ¯·½Ê½ ²éÑ¯ SRI Èç¹ûÊÇÔÚÖÐ¶Ï·½Ê½ ÔÚEIE1.6 ±È½ÏÆ÷1ÖÐ¶Ï·þÎñ³ÌÐòÖÐ²éÑ¯ SRI
// 2) ´Ó RDR ¶ÁÊý¾Ý  
//  
 
#include "SWSerial.h"
#include "pin_define.h"

bit SRI;                           //½ÓÊÕÍê³É±êÖ¾
bit STI;                           //·¢ËÍÍê³É±êÖ¾
bit STXBSY;                        //·¢ËÍÃ¦±êÖ¾
bit SREN;                          //½ÓÊÕÔÊÐí
bit	SES;						   //SW_UART ÓÃ»§¼¶ÖÐ¶ÏÊ¹ÄÜÎ»
 
//sbit SW_RX = P1^4;                   //½ÓÊÕÒý½Å
//sbit SW_TX = P1^5;                   //·¢ËÍÒý½Å
 
char TDR;                          //·¢ËÍÊý¾Ý¼Ä´æÆ÷
char RDR;                          //½ÓÊÕÊý¾Ý¼Ä´æÆ÷

bit SW_DONE;                       //SW·¢ËÍ½áÊø±êÖ¾£¨·¢ËÍÍê15¸ö×Ö·û£©

//²âÊÔ±äÁ¿
//char k,m;                          //²âÊÔ¼ÆÊýÆ÷
//char idata SW_BUF[15];             //²âÊÔ½ÓÊÕ»º³åÇø
 
void PCA_ISR();                     //SW_UARTÖÐ¶Ï·þÎñ³ÌÐò 

/******************************************************************************
** º¯ Êý Ãû: void SW_UART_INIT(void)
** ¹¦ÄÜÃèÊö: Èí¼þ´®¿Ú³õÊ¼»¯
** Êä¡¡  Èë: ÎÞ
** Êä¡¡  ³ö: ÎÞ
** È«¾Ö±äÁ¿: ÎÞ
** µ÷ÓÃÄ£¿é: ÎÞ
** ×÷¡¡  Õß: xzy
** ÈÕ¡¡  ÆÚ: 2013.05
******************************************************************************/ 
void SW_UART_INIT(void)
{
	PCA0CPM0 = 0x10;            //Ä£¿é0Îª¸ºÑØ²¶×½·½Ê½£¬½ûÖ¹Ä£¿é0ÖÐ¶Ï										
	PCA0CPM1 = 0x48;            //Ä£¿é1ÎªÈí¼þ¶¨Ê±Æ÷·½Ê½£¬½ûÖ¹Ä£¿é1ÖÐ¶Ï
	
	PCA0CN = 0;                 //PCA±£³Ö½ûÖ¹×´Ì¬              
	PCA0MD = 0x02;              //PCAÊ±»ù=SYSCLK/4;½ûÖ¹PCA¼ÆÊýÆ÷ÖÐ¶Ï
	
	CCF0 = 0;                   //Çå³ýPCAÄ£¿é0ºÍ1²¶×½±È½ÏÄ£¿é
	CCF1 = 0; 
	                
	SRI = 0;                   //Çå³þ½ÓÊÜÍê³É±êÖ¾
	STI = 0;                    //Çå³ý·¢ËÍÍê³É±êÖ¾
	
	SW_TX = 1;                  //½«TXÏß³õÊ¼»¯Îª¸ßµçÆ½
	STXBSY = 0;                 //Çå³ýSW_UARTÃ¦±êÖ¾
}
//----------------------------------------------------------------------------------
/******************************************************************************
** º¯ Êý Ãû: void SW_UART_ENEABLE(void)
** ¹¦ÄÜÃèÊö: Èí¼þ´®¿ÚÊ¹ÄÜ
** Êä¡¡  Èë: ÎÞ
** Êä¡¡  ³ö: ÎÞ
** È«¾Ö±äÁ¿: ÎÞ
** µ÷ÓÃÄ£¿é: ÎÞ
** ×÷¡¡  Õß: xzy
** ÈÕ¡¡  ÆÚ: 2013.
******************************************************************************/ 
void SW_UART_ENEABLE(void)
{ 
	PCA0CPM0 |= 0x01;           //ÔÊÐíPCAÄ£¿é0£¨½ÓÊÕÖÐ¶Ï£©
	PCA0CPM1 |= 0x01;           //ÔÊÐíPCAÄ£¿é1£¨·¢ËÍÖÐ¶Ï£©	
	CR = 1;                    //Æô¶¯PCA¼ÆÊýÆ÷
	PCA0IntSwitchOn();        //ÔÊÐíPCAÖÐ¶Ï
	//GlobleIntSwitchOn();       //È«¾ÖÖÐ¶ÏÔÊÐí
	CPT1MD = 0x10;				//±È½ÏÆ÷ 1 ÏÂ½µÑØÖÐ¶ÏÔÊÐí 
	SREN = 1;					// Ê¹ÄÜÊý¾Ý½ÓÊÕ
	SES = 1;					// ÓÃ»§¼¶ÖÐ¶ÏÊ¹ÄÜ
}
/******************************************************************************
** º¯ Êý Ãû: void SW_UART_DISABLE(void)
** ¹¦ÄÜÃèÊö: ¹Ø±ÕÈí¼þ´®¿Ú
** Êä¡¡  Èë: ÎÞ
** Êä¡¡  ³ö: ÎÞ
** È«¾Ö±äÁ¿: ÎÞ
** µ÷ÓÃÄ£¿é: ÎÞ
** ×÷¡¡  Õß: xzy
** ÈÕ¡¡  ÆÚ: 2013.
******************************************************************************/ 
void SW_UART_DISABLE(void)
{ 
	PCA0CPM0 &= (~0x01);           //½ûÓÃPCAÄ£¿é0£¨½ÓÊÕÖÐ¶Ï£©
	PCA0CPM1 &= (~0x01);           //½ûÓÃPCAÄ£¿é1£¨·¢ËÍÖÐ¶Ï£©	
	CR = 0;                    //½ûÓÃPCA¼ÆÊýÆ÷
	PCA0IntSwitchOff();        //½ûÓÃPCAÖÐ¶Ï
	//GlobleIntSwitchOn();       //È«¾ÖÖÐ¶ÏÔÊÐí
	CPT1MD &= (~0x10);				//±È½ÏÆ÷ 1 ÏÂ½µÑØÖÐ¶ÏÔÊÐí 
	SREN = 0;					// ½ûÓÃÊý¾Ý½ÓÊÕ
	SES = 0;					// ÓÃ»§¼¶ÖÐ¶ÏÊ¹½ûÖ¹
}
/******************************************************************************
** º¯ Êý Ãû: void StartSend(void)
** ¹¦ÄÜÃèÊö: Æô¶¯Èí¼þ´®¿Ú·¢ËÍ
** Êä¡¡  Èë: ÎÞ
** Êä¡¡  ³ö: ÎÞ
** È«¾Ö±äÁ¿: ÎÞ
** µ÷ÓÃÄ£¿é: ÎÞ
** ×÷¡¡  Õß: xzy
** ÈÕ¡¡  ÆÚ: 2013.
******************************************************************************/ 
void StartSend(void)
{
	BufDat->NumCount = 0;
	STI=1;
	SW_DONE = 0;
	CP1IntSwitchOn();   // ÔÊÐí±È½ÏÆ÷1£¨CP1£©ÖÐ¶Ï
	CPT1CN |= 0x10;    // ´¥·¢±È½ÏÆ÷ 1 ÏÂ½µÑØÖÐ¶Ï
}
//-------------------------------------------------------------------------------------
//ÖÐ¶Ï·þÎñ³ÌÐò
//--------------------------------------------------------------------------------------
void PCA_ISR(void) interrupt PCA0_PRIOR_ORDER			 //11
 {
  static char SUTXST = 0;           //SW_UART TX×´Ì¬±äÁ¿
  static char SURXST = 0;           //SW_UART RX×´Ì¬±äÁ¿
  static unsigned char RXSHIFT;   //SW_UART RXÒÆÎ»¼Ä´æÆ÷
  static int PCA_TEMP;            //ÁÙÊ±´¢´æ±äÁ¿£¬ÓÃÓÚ´¦ÀíPCAÄ£¿éµÄ¸ßºÍµÍ×Ö½Ú

//Ê×ÏÈ¼ì²é½ÓÊÜÖÐ¶Ï±äÁ¿£¬Èç¹ûCCF0ÖÃÎ»Ôò¶ÔÆä·þÎñ 
  if(CCF0){
     CCF0=0;                      //Çå³ýÖÐ¶Ï±êÖ¾
     switch(SURXST){
			//×´Ì¬£º0 ÊÕµ½ÆðÊ¼Î»
			//ÔÚ¸Ã×´Ì¬£¬ÊÇSW_TXÉÏµÄ¸º±ßÑØ´¥·¢ÖÐ¶Ï£¬±íÊ¾¼ì²âµ½ÆðÊ¼Î»£¬Í¬Ê±PCA0CP0¼Ä´æÆ÷²¶×½PCA0µÄÖµ
			//¼ì²é½ÓÊÜÔÊÐíºÍÆðÊ¼Î»
			//½«PCAÄ£¿é0ÇÐ»»µ½Èí¼þ¶¨Ê±·½Ê½
			//¼Ó3/2Î»Ê±¼äµ½Ä£¿é0²¶×½¼Ä´æÆ÷ÒÔ²ÉÑùLSB
			//TX×´Ì¬±äÁ¿Ó1
            case 0:
                 if(SREN & ~SW_RX){  //¼ì²é½ÓÊÕÔÊÐíºÍÆðÊ¼Î»

                    PCA_TEMP = (PCA0CPH0<<8);//½«Ä£¿é0µÄÄÚÈÝ¶Áµ½PCA_TEMP
                    PCA_TEMP |= PCA0CPL0;
                    PCA_TEMP += TH_TIME_COUNT;//¼Ó3/2Î»Ê±¼äµ½PCA_TEMP

                    PCA0CPL0 = PCA_TEMP;     //¸üÐÂPCA0CPL0ºÍPCA0CPH0
                    PCA0CPH0 = (PCA_TEMP>>8);

                    PCA0CPM0=0x49;         //½«Ä£¿é0ÇÐ»»µ½Èí¼þ¶¨Ê±Æ÷·½Ê½£¬ÔÊÐíÖÐ¶Ï

                    SURXST++;              //¸üÐÂRX×´Ì¬±äÁ¿  
					}              
                    break;
			//×´Ì¬1-8£ºÊÕµ½Êý¾ÝÎ»
			//²ÉÑùSW_RXÒý½Å
			//½«ÐÂÊý¾ÝÒÆÈëRXSHIFT
			//¼Ó1¸öÎ»Ê±¼ä±äÁ¿µ½Ä£¿é0²¶×½¼Ä´æÆ÷
			//RX×´Ì¬±äÁ¿Ó1
             case 1:             
             case 2:
             case 3:              
             case 4:            
             case 5:
             case 6: 
             case 7:
             case 8:
                 RXSHIFT = RXSHIFT >> 1; //ÓÒÒÆÒ»Î»
                 if(SW_RX)             //Èç¹ûSW_RX=1£»½«1ÒÆÈëRXSHIFTµÄMSB
                    RXSHIFT |= 0x80;
                   
                    PCA_TEMP = (PCA0CPH0<<8);//½«Ä£¿é0ÄÚÈÝ¶Áµ½PCA_TEMP
                    PCA_TEMP |= PCA0CPL0;

                    PCA_TEMP += TIME_COUNT;//¼Ó1¸öÎ»Ê±¼äµ½PCA_TEMP

                    PCA0CPL0 = PCA_TEMP;   //¸üÐÂPCA0CPL0ºÍPCA0CPH0
                    PCA0CPH0 = (PCA_TEMP>>8);

                    SURXST++;              //¸üÐÂRX×´Ì¬±äÁ¿
                    break;
			//×´Ì¬9£ºÒÑÊÕµ½8¸öÊý¾ÝÎ»£¬²¶×½Í£Ö¹Î»
			//½«RXSHIFT´«µ½RDR
			//ÖÃÎ»SRI£¨±íÊ¾½ÓÊÕÍê³É£©
			//ÉèÖÃÄ£¿é0£¬ÎªÏÂÒ»´Î´«Êä×ö×¼±¸
			//¸´Î»RX×´Ì¬±äÁ¿
            case 9:
                 RDR = RXSHIFT;
                 SRI = 1;
                 PCA0CPM0 = 0x11;
                 SURXST = 0;
				 if (SES){      //Èç¹ûÓÃ»§¼¶ÖÐ¶ÏÖ§³Ö±»ÔÊÐí 
					CP1IntSwitchOn();   // ÔÊÐí±È½ÏÆ÷1£¨CP1£©ÖÐ¶Ï
					CPT1CN |= 0x10;    // ´¥·¢±È½ÏÆ÷ 1 ÏÂ½µÑØÖÐ¶Ï
				}
                 break;
             }
 }             
//¼ì²é·¢ËÍÖÐ¶Ï£¬Èç¹ûCCF1ÖÃÎ»Ôò¶ÔÆä·þÎñ
     else if(CCF1){
          CCF1=0;              //Çå³ýÖÐ¶Ï±êÖ¾
          switch(SUTXST){
		//×´Ì¬0£º·¢ËÍ¹ý³ÌÒÔÆô¶¯
		//ÔÚ´Ë£¬ÓÃ»§ÒÑ½«Òª·¢ËÍµÄ×Ö½Ú×°Èëµ½TDR£¬Ç¿ÖÆÄ£¿é1ÖÐ¶ÏÒÔÆô¶¯·¢ËÍ
		//·¢ËÍÆðÊ¼Î»£¨Ê¹SW_TX±äµÍµçÆ½£©
		//¶ÁPCA0,¼ÓÒ»¸öÎ»Ê±¼äºó´æµ½Ä£¿é1²¶×½¼Ä´æÆ÷
		//TX×´Ì¬±äÁ¿¼Ó1
          case 0:
               SW_TX = 0;				  // Ê¹ TX Òý½Å±äµÍ×÷ÎªÆðÊ¼Î» 
               PCA_TEMP = PCA0L;		  // ½« PCA ¼ÆÊýÆ÷µÄÖµ¶Áµ½ PCA_TEMP  
               PCA_TEMP |= (PCA0H<<8);
               PCA_TEMP += TIME_COUNT;	  // ¼ÓÒ»¸öÎ»Ê±¼ä  
               PCA0CPL1 = PCA_TEMP;;	  // ½«¸üÐÂºóµÄÆ¥ÅäÖµ´æµ½ 
               PCA0CPH1 = (PCA_TEMP>>8);  // Ä£¿é 1 ²¶×½±È½Ï¼Ä´æÆ÷
               PCA0CPM1 |= 0x48;		  // ÔÊÐíÄ£¿é 1 Èí¼þ¶¨Ê±Æ÷
               SUTXST++;				  // ¸üÐÂ TX ×´Ì¬±äÁ¿ 
               break;
		//×´Ì¬1-9£º·¢ËÍÊý¾ÝÎ»
		//½«TDRµÄLSBÊäÈëµ½TX
		//½«TDRÓÒÒÆÒ»Î»
		//½«Ò»¸ö1ÒÆÈëTDRµÄMSB×÷Îª×´Ì¬9µÄÍ£Ö¹Î»
		//¼ÓÒ»¸öÊ±¼äµ½Ä£¿é1²¶×½¼Ä´æÆ÷
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 9:
               SW_TX = (TDR & 0x01);   // ½«TDRµÄLSBÊä³öµ½SW_TXÒý½Å  
               TDR >>= 1;			   // TDR ÓÒÒÆÒ»Î»  
               TDR |= 0x80;			   // ½«Ò»¸ö'1'ÒÆÈëTDRµÄMSB×÷Îª×´Ì¬9µÄÍ£Ö¹Î» 
               PCA_TEMP = (PCA0CPH1<<8);  // ½«Ä£¿é 0 ÄÚÈÝ¶Áµ½PCA_TEMP  
               PCA_TEMP |= PCA0CPL1;
               PCA_TEMP += TIME_COUNT; // ¼ÓÒ»¸öÎ»Ê±¼äµ½PCA_TEMP 
               PCA0CPL1 = PCA_TEMP;;   // ÓÃÐÂÖµ»Ö¸´ PCA0CPL1ºÍPCA0CPH1 
               PCA0CPH1 = (PCA_TEMP>>8);
    
               SUTXST++;				 // ¸üÐÂ TX ×´Ì¬±äÁ¿
               break;
		//×´Ì¬10£º×îºóÒ»Î»Êý¾ÝÒÑ·¢ËÍÍê¡£·¢ËÍÍ£Ö¹Î»²¢½áÊø´«Êä¹ý³Ì
		//·¢ËÍÍ£Ö¹Î»
		//ÖÃ1·¢ËÍ½áÊø±êÖ¾£¬Çå³ýÃ¦±êÖ¾
		//¸´Î»TX×´Ì¬
		//ÉèÖÃÄ£¿é1£¬ÎªÏÂÒ»´Î´«Êä×ö×¼±¸
        case 10:
              STI = 1;					 // ±íÊ¾·¢ËÍÍê³É
              SUTXST = 0;				 // ¸´Î» TX ×´Ì¬  
              SW_TX = 1;				 // SW_TX Ó¦±£³Ö¸ßµçÆ½  
              PCA0CPM1 = 0x01;			 // ½ûÖ¹Ä£¿é 1 Èí¼þ¶¨Ê±Æ÷,±£³ÖÖÐ¶ÏÎªÔÊÐí×´Ì¬ÒÔ±¸ÏÂÒ»´Î´«Êä
              if (SES){      			//Èç¹ûÓÃ»§¼¶ÖÐ¶ÏÖ§³Ö±»ÔÊÐí 
				CP1IntSwitchOn();   	// ÔÊÐí±È½ÏÆ÷1£¨CP1£©ÖÐ¶Ï
				CPT1CN |= 0x10;    		// ´¥·¢±È½ÏÆ÷ 1 ÏÂ½µÑØÖÐ¶Ï  
				}
			  	STXBSY = 0;		   		// SW_UART TX ¿ÕÏÐ
			  break;
            }
        }
   
}
//-------------------------------------------------------------------- 
// USER_ISR: ½«±È½ÏÆ÷1ÖÐ¶ÏÓÃ×÷ÓÃ»§ SW_UART ÖÐ¶Ï·þÎñ³ÌÐò(EIE1.6 ±È½ÏÆ÷1ÖÐ¶Ï) 
// Èç¹ûÖÐ¶Ï²âÊÔ·½Ê½±»ÔÊÐí ¸Ã ISR ½«·¢ËÍ 15 ¸ö×Ö·û²¢½ÓÊÕ 15 ¸ö×Ö·û  
// Ã¿´Î SW_UART ·¢ËÍ»ò½ÓÊÕÍê³É¶¼Òª´¥·¢¸ÃÀý³Ì  
// - ¼ì²é½ÓÊÕÍê³ÉÖ¸Ê¾±êÖ¾²¢·þÎñ  
// - ¼ì²é·¢ËÍÍê³ÉÖ¸Ê¾±êÖ¾²¢·þÎñ  
// - ¼ì²é ISR Ö´ÐÐÆÚ¼äÊÇ·ñÓÐ·¢ËÍºÍ½ÓÊÕ¹ý³ÌÍê³É ÈçÓÐ ÔÙ´¥·¢Ò»´ÎÖÐ¶Ï  
// 
void USER_ISR(void) interrupt COMPARATOR1_PRIOR_ORDER 	  // 13 ÖÐ¶Ï·þÎñ³ÌÐò
{     
	CPT1CN &= ~(0x10);       // Çå±È½ÏÆ÷1ÏÂ½µÑØÖÐ¶Ï±êÖ¾ 
	if (SRI){         // Èç¹û½ÓÊÕÍê³É: 
		SRI = 0;         // Çå³ý½ÓÊÕ±êÖ¾  		
		//BufDat->NumCount ++;						 //½ÓÊÕ¼ÆÊý
		if (BufDat->NumCount >= BUF_NUM){        // Èç¹û½ÓÊÕÂú 
			BufDat->NumCount = 0;
			//BufDat->BitPoint = 0;
			//SREN=0;        // ½ûÖ¹ SW_UART ½ÓÊÕÆ÷ 
		}
		BufDat->Dat[BufDat->NumCount] = RDR;      // ¶Á½ÓÊÕ»º³åÆ÷ 
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
			BufDat->NumCount ++;						 //½ÓÊÕ¼ÆÊý
		}
				          
	}  
	else if (STI){        // Èç¹û·¢ËÍÍê³É: 
		STI = 0;         // Çå³ý·¢ËÍ±êÖ¾  
		if (BufDat->NumCount < BufDat->BitPoint){             // Èç¹û×Ö·ûÎ´·¢ËÍÍê: 
			STXBSY = 1;                       // ÉêÇë SW_UART ·¢ËÍÆ÷  
			TDR = BufDat->Dat[BufDat->NumCount++];            // ±äÁ¿¼Ó 1 ·¢ËÍ
			CCF1 = 1;         // Ç¿ÖÆÄ£¿é 1 ÖÐ¶ÏÒÔÆô¶¯·¢ËÍ 
		} 
		else
		{ 
			BufDat->NumCount = 0;
			BufDat->BitPoint = 0;
			SREN = 1;					// Ê¹ÄÜÊý¾Ý½ÓÊÕ
			SW_DONE=1;              // ±íÊ¾ÒÑ·¢ËÍÍê×îºóÒ»¸ö×Ö·û
		}  
	}  
//	if (STI|SRI)         // Èç¹û SRI »ò STI ÖÃÎ» ÔÙ´Î´¥·¢ 
//	CPT1CN |= 0x10;    		// ´¥·¢±È½ÏÆ÷ 1 ÏÂ½µÑØÖÐ¶Ï 
}
