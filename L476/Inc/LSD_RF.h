////////////////////////////////////////////////////////////////////////////////
// ��Ȩ:     ������Ƽ����Źɷ����޹�˾
// �ļ���:   LSD_RF.h
// �汾��    v1.0
// ��������: IAR  v5.30
// ����:     �ں���
// ��������: 2014.11
// ����:     
// ����ļ�: 
// �޸���־��
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _LSD_RF_h_
#define _LSD_RF_h_
#include "spi.h"
#include "gpio.h"
////===================================RFӲ��IO�ڶ��壨����ʵ��ʹ���޸ģ�===================================================

////SX1276 SPI I/O definitions
////SIMO
//#define  LSD_SPI_SIMO_PORT_SEL   P6SEL_bit.P2
//#define  LSD_SPI_SIMO_PORT_DIR   P6DIR_bit.P2
//#define  LSD_SPI_SIMO_PORT_OUT   P6OUT_bit.P2
//#define  LSD_SPI_SIMO_PORT_IN    P6IN_bit.P2
////SCK
//#define  LSD_SPI_SCK_PORT_SEL    P6SEL_bit.P4
//#define  LSD_SPI_SCK_PORT_DIR    P6DIR_bit.P4
//#define  LSD_SPI_SCK_PORT_OUT    P6OUT_bit.P4
//#define  LSD_SPI_SCK_PORT_IN     P6IN_bit.P4   
////SOMI
//#define  LSD_SPI_SOMI_PORT_SEL   P6SEL_bit.P1
//#define  LSD_SPI_SOMI_PORT_DIR   P6DIR_bit.P1
//#define  LSD_SPI_SOMI_PORT_OUT   P6OUT_bit.P1
//#define  LSD_SPI_SOMI_PORT_IN    P6IN_bit.P1
////NSS
//#define  LSD_SPI_NSS_PORT_SEL    P6SEL_bit.P3
//#define  LSD_SPI_NSS_PORT_DIR    P6DIR_bit.P3
//#define  LSD_SPI_NSS_PORT_OUT    P6OUT_bit.P3
//#define  LSD_SPI_NSS_PORT_IN     P6IN_bit.P3

//////SX1276 DIO pins  I/O definitions
//////DIO0
extern uint8_t GPIO_PIN_7_exit_tp;

#define LSD_RF_DIO0_SEL      _DEFINE          //DIO0 IO���ù���ѡ�� 
#define LSD_RF_DIO0_DIR      _DEFINE           //DIO0 ����Ĵ���   
#define LSD_RF_DIO0_IFG      GPIO_PIN_7_exit_tp          //DIO0 �жϱ�־λ�Ĵ���
#define LSD_RF_DIO0_IES      _DEFINE           //DIO0 ������ʽ�Ĵ����������ػ��½���
#define LSD_RF_DIO0_IE       _DEFINE            //DIO0 �ж�ʹ�ܼĴ���
#define LSD_RF_DIO0_IN       _DEFINE            //DIO0 ����Ĵ���
//#define LSD_RF_DIO0_SEL            P1SEL_bit.P0     //DIO0 IO���ù���ѡ�� 
//#define LSD_RF_DIO0_DIR            P1DIR_bit.P0     //DIO0 ����Ĵ���   
//#define LSD_RF_DIO0_IFG            P1IFG_bit.P0     //DIO0 �жϱ�־λ�Ĵ���
//#define LSD_RF_DIO0_IES            P1IES_bit.P0     //DIO0 ������ʽ�Ĵ����������ػ��½���
//#define LSD_RF_DIO0_IE             P1IE_bit.P0      //DIO0 �ж�ʹ�ܼĴ���
//#define LSD_RF_DIO0_IN             P1IN_bit.P0      //DIO0 ����Ĵ���
////DIO1��δʹ�ã�
//#define LSD_RF_DIO1_SEL            P5SEL_bit.P0     //DIO1 IO���ù���ѡ��  
//#define LSD_RF_DIO1_DIR            P5DIR_bit.P0     //DIO1 ����Ĵ���   
//#define LSD_RF_DIO1_IFG            P5IFG_bit.P0     //DIO1 �жϱ�־λ�Ĵ���
//#define LSD_RF_DIO1_IES            P5IES_bit.P0     //DIO1 ������ʽ�Ĵ����������ػ��½���
//#define LSD_RF_DIO1_IE             P5IE_bit.P0      //DIO1 �ж�ʹ�ܼĴ���
//#define LSD_RF_DIO1_IN             P5IN_bit.P0      //DIO1 ����Ĵ���
////DIO2
//#define LSD_RF_DIO2_SEL            P1SEL_bit.P1     //DIO2 IO���ù���ѡ��  
//#define LSD_RF_DIO2_DIR            P1DIR_bit.P1     //DIO2 ����Ĵ���   
//#define LSD_RF_DIO2_IFG            P1IFG_bit.P1     //DIO2 �жϱ�־λ�Ĵ���
//#define LSD_RF_DIO2_IES            P1IES_bit.P1     //DIO2 ������ʽ�Ĵ����������ػ��½���
//#define LSD_RF_DIO2_IE             P1IE_bit.P1      //DIO2 �ж�ʹ�ܼĴ���
//#define LSD_RF_DIO2_IN             P1IN_bit.P1      //DIO2 ����Ĵ���
////DIO3��δʹ�ã�
#define LSD_RF_DIO3_SEL            _DEFINE     //DIO3 IO���ù���ѡ��  
#define LSD_RF_DIO3_DIR            _DEFINE     //DIO3 ����Ĵ���   
#define LSD_RF_DIO3_IFG            _DEFINE     //DIO3 �жϱ�־λ�Ĵ���
#define LSD_RF_DIO3_IES            _DEFINE     //DIO3 ������ʽ�Ĵ����������ػ��½���
#define LSD_RF_DIO3_IE             _DEFINE      //DIO3 �ж�ʹ�ܼĴ���
#define LSD_RF_DIO3_IN             _DEFINE      //DIO3 ����Ĵ���
//#define LSD_RF_DIO3_SEL            P2SEL_bit.P0     //DIO3 IO���ù���ѡ��  
//#define LSD_RF_DIO3_DIR            P2DIR_bit.P0     //DIO3 ����Ĵ���   
//#define LSD_RF_DIO3_IFG            P2IFG_bit.P0     //DIO3 �жϱ�־λ�Ĵ���
//#define LSD_RF_DIO3_IES            P2IES_bit.P0     //DIO3 ������ʽ�Ĵ����������ػ��½���
//#define LSD_RF_DIO3_IE             P2IE_bit.P0      //DIO3 �ж�ʹ�ܼĴ���
//#define LSD_RF_DIO3_IN             P2IN_bit.P0      //DIO3 ����Ĵ���
////DIO4��δʹ�ã�
//#define LSD_RF_DIO4_SEL            P2SEL_bit.P1     //DIO4 IO���ù���ѡ��  
//#define LSD_RF_DIO4_DIR            P2DIR_bit.P1     //DIO4 ����Ĵ���   
//#define LSD_RF_DIO4_IFG            P2IFG_bit.P1     //DIO4 �жϱ�־λ�Ĵ���
//#define LSD_RF_DIO4_IES            P2IES_bit.P1     //DIO4 ������ʽ�Ĵ����������ػ��½���
//#define LSD_RF_DIO4_IE             P2IE_bit.P1      //DIO4 �ж�ʹ�ܼĴ���
//#define LSD_RF_DIO4_IN             P2IN_bit.P1      //DIO4 ����Ĵ���

////SX1276 RESET I/O definitions
//#define  LSD_RF_SXRESET_DIR        P5DIR_bit.P1
//#define  LSD_RF_SXRESET_OUT        P5OUT_bit.P1

//////==========================================end======================================================



//////================================�����궨��======================================================
//#define  LSD_RF_DELAY()            __no_operation()             //SPI��ʱ
#define LSD_RF_SPIInit()
//#define LSD_RF_SPIInit()        do{         \
//                                        LSD_SPI_SIMO_PORT_SEL=0 ;\
//                                        LSD_SPI_SIMO_PORT_DIR=1 ;\
//                                        LSD_SPI_SIMO_PORT_OUT=1; \
//                                            \
//                                        LSD_SPI_SCK_PORT_SEL=0;  \
//                                        LSD_SPI_SCK_PORT_DIR=1;  \
//                                        LSD_SPI_SCK_PORT_OUT=0;  \
//                                           \
//                                        LSD_SPI_SOMI_PORT_SEL=0; \
//                                        LSD_SPI_SOMI_PORT_DIR=0; \
//                                        LSD_SPI_SOMI_PORT_OUT=0; \
//                                           \
//                                        LSD_SPI_NSS_PORT_SEL=0 ; \
//                                        LSD_SPI_NSS_PORT_DIR=1;  \
//                                        LSD_SPI_NSS_PORT_OUT=1;  \
//                                  }while(0)

//#define LSD_SPI_SIMO_OUT(x)   LSD_SPI_SIMO_PORT_OUT=x            //SIMO=X
//#define LSD_SPI_SCK_OUT(x)    LSD_SPI_SCK_PORT_OUT=x             //sck=x
//#define LSD_SPI_NSS_OUT(x)    LSD_SPI_NSS_PORT_OUT=x             //nss=x
//#define LSD_SPI_SOMI_IN()     LSD_SPI_SOMI_PORT_IN            

#define LSD_SPI_NSS_OUT(x)			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, x)


/////*********************************************************************
////**  function Declaration
////*********************************************************************/
void LSD_RF_StandbyMode(void);
void LSD_RF_SleepMode(void);
void LSD_RF_Config(void);
void LSD_RF_RestRF(void);
unsigned char LSD_RF_InitRF(void);
void LSD_RF_RXmode(void);
void LSD_RF_SendPacket(uint8_t *txBuffer, uint8_t size);
void LSD_RF_RxPacket(uint8_t *cRxBuf, uint8_t *cLength);
void LSD_RF_FreqSet(uint8_t ch);
void LSD_RF_WORInit(void);
void LSD_RF_WORexecute(uint8_t cLength);
void LSD_RF_WOR_Exit(void);
void LSD_RF_AwakePkt(void);
//void LSD_RF_RSSIInit(void);
void LSD_RF_TriggerRssi(void);
unsigned char LSD_RF_GetRSSI(void);
void LSD_RF_test(void);
#endif
