#include "LSD_RFreg.h"
#include "LSD_RF.h"
#include "LSD_RFregsetting.h"
//extern uint8_t Sleep_RX;

uint8_t _DEFINE=0;

uint8_t RF_Version;

#define DelayMs HAL_Delay

//**********************************************************************************
// �������� : spi����or����1�ֽ�
// ������� : unsigned char mosi
// ���ز��� : unsigned char
// ˵��     : 
//**********************************************************************************
static unsigned char LSD_RF_SpiInOut (unsigned char mosi) 
{ 
	uint8_t miso=0;
	HAL_SPI_TransmitReceive(&hspi1,&mosi,&miso,1,0xfff);
	return miso;
//    unsigned char i;
//    unsigned char onebyte;
//    onebyte = mosi;
//    for(i = 8; i > 0; i--) 
//    {
//        LSD_RF_DELAY();
//        if(onebyte & 0x80) 
//        {
//            LSD_SPI_SIMO_OUT(1);
//        }
//        else 
//        {
//            LSD_SPI_SIMO_OUT(0);
//        }

//        LSD_RF_DELAY();
//        LSD_SPI_SCK_OUT(1);
//        onebyte <<= 1;      // next bit
//        if(LSD_SPI_SOMI_IN()) 
//        {
//            onebyte++;      // 1 found on MISO
//        }
//        LSD_RF_DELAY();
//        LSD_SPI_SCK_OUT(0);
//    }
//    return onebyte;         // mosi now becomes the value of miso
}
//**********************************************************************************
// �������� : spi��д�Ĵ���
// ������� : unsigned char addr��ַ unsigned char val����
// ���ز��� : 
// ˵��     : Note: don't call it directely use macro SpiWriteReg and SpiWriteReg  to call this routine
//**********************************************************************************
unsigned char LSD_RF_SpiRW_Reg (unsigned char addr, unsigned char val) 
{
    unsigned char rc;
    LSD_RF_SPIInit();
    LSD_SPI_NSS_OUT(GPIO_PIN_RESET);
    LSD_RF_SpiInOut(addr);
    rc = LSD_RF_SpiInOut(val);
    LSD_SPI_NSS_OUT(GPIO_PIN_SET);
    return rc;
}
//**********************************************************************************
// �������� : д�Ĵ���
// ������� : addr��ַ val����
// ���ز��� : ��
// ˵��     : 
//**********************************************************************************
#define LSD_RF_WriteReg(addr, val)         LSD_RF_SpiRW_Reg(addr|0x80,val)
//**********************************************************************************
// �������� : ���Ĵ���
// ������� : addr��ַ
// ���ز��� : �Ĵ���ֵ
// ˵��     : 
//**********************************************************************************
#define LSD_RF_ReadReg(addr)               LSD_RF_SpiRW_Reg(addr&0x7F, 0)
//**********************************************************************************
// �������� : ���FIFO�Լ������־λ
// ������� : 
// ���ز��� : 
// ˵��     : 
//**********************************************************************************
#define LSD_RF_ClearFIFO()                    LSD_RF_WriteReg(REG_IRQFLAGS2, 0x10)
//**********************************************************************************
// �������� : ȷ������standby
// ������� : 
// ���ز��� : 
// ˵��     : ע�⣺�˺�����listen���ܹر�,TS_OSC>500us
//**********************************************************************************
void LSD_RF_StandbyMode(void)
{
    unsigned long int i=600000;
    LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY));
    while (((LSD_RF_ReadReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00)&&i)i--; // Wait for ModeReady
}
//**********************************************************************************
// �������� : ȷ������sleep
// ������� : 
// ���ز��� : 
// ˵��     : ע�⣺�˺�����listen���ܹر�
//**********************************************************************************
void LSD_RF_SleepMode(void)
{
    unsigned long int i=600000;
    LSD_RF_StandbyMode();       //���л���standbyģʽ���е�sleepģʽ
    LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_SLEEP));
    while (((LSD_RF_ReadReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00)&&i)i--; // Wait for ModeReady
//    LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00+RF_DIOMAPPING1_DIO1_11+RF_DIOMAPPING1_DIO2_01+RF_DIOMAPPING1_DIO3_01); // DIO0 = packet sent
//    LSD_RF_WriteReg(REG_DIOMAPPING2, RF_DIOMAPPING2_DIO4_00+RF_DIOMAPPING2_DIO5_00); // DIO0 = packet sent
    LSD_SPI_NSS_OUT(GPIO_PIN_RESET);  //����͹��ģ�Ҫ��NSS���ͣ���ֹ©�������������Ҫ
}
//**********************************************************************************
// �������� : ȷ������RXģʽ
// ������� : 
// ���ز��� : 
// ˵��     : ע�⣺�˺�����listen���ܹر�  ��DIO3=Premble   DIO0=PLD
//**********************************************************************************
void LSD_RF_RXmode(void)
{
    unsigned char nTimes,cData;
    LSD_RF_DIO0_IFG=0;          //���MCU��־λ
    LSD_RF_DIO3_IFG=0;          //���MCU��־λ
    LSD_RF_DIO0_IE=0;           //�������ǰ�ر��ж�
    LSD_RF_DIO3_IE=0;           //���ж�
    LSD_RF_StandbyMode();       //���л���standbyģʽ
    if(RF_Version==RF_SX1208)
       LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAP1_DIO0_PLD_RDY|RF_DIOMAPPING1_DIO3_01); // DIO0 = PLD_RDY
    else 
       LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAP1_DIO0_PLD_RDY|RF_DIOMAPPING1_DIO3_10); // DIO0 = PLD_RDY
    LSD_RF_ClearFIFO();         //��FIFO  
    while(1)    //��ֹ�ⲿ���ص���SPI�л���RXģʽʧ�ܣ��˴���Ϊ�������á�
    {
         nTimes=100;
         LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_RECEIVER));
         //�ȴ��������״̬
         do
         {
             cData=LSD_RF_ReadReg(REG_OPMODE) & 0x1c;
             nTimes--;
         }
         while((cData!=RF_OPMODE_RECEIVER)&&nTimes);
         if(!nTimes)
              continue;
         LSD_RF_DIO0_IFG=0;          //���MCU��־λ
         LSD_RF_DIO3_IFG=0;          //���MCU��־λ
         LSD_RF_DIO0_IE=1;           //��ʼ�ж�
         LSD_RF_DIO3_IE=1;           //��ʼ�ж�
         return;
     } 
}
////**********************************************************************************
//// �������� : д���ñ�
//// ������� : ��
//// ���ز��� : ��
//// ˵��     : 
////**********************************************************************************
void LSD_RF_Config(void)
{
    _SX12XX_REG const *p;
    unsigned char i;
    p = LSD_RFregConfig;
    for(i=sizeof(LSD_RFregConfig)/2; i>0; i--) 
    {
        LSD_RF_WriteReg(p->addr, p->val);
        p++;
    }
}
////**********************************************************************************
//// �������� : ��λ����ģ��
//// ������� : ��
//// ���ز��� : ��
//// ˵��     : 
////**********************************************************************************
void LSD_RF_RestRF(void)
{
//   LSD_RF_SXRESET_DIR =1;  //��λ���������
//   LSD_RF_SXRESET_OUT =1;  //��λ��������ߵ�ƽ��λ
//   DelayMs(20);
//   LSD_RF_SXRESET_OUT=0;   //��λ��������͵�ƽ
//   DelayMs(20);
}
////**********************************************************************************
//// �������� : RF��ʼ��
//// ������� : ��
//// ���ز��� : ��
//// ˵��     : 
////**********************************************************************************
unsigned char LSD_RF_InitRF(void) 
{
    LSD_RF_RestRF();                    //��λRF
    LSD_RF_SPIInit();                   //RF��ʼ�� 
    LSD_RF_StandbyMode();               //����standbyģʽ
    LSD_RF_Config();                    //дRF���ñ������standbyģʽ��sleepģʽ���޸�
    RF_Version = LSD_RF_ReadReg(REG_VERSION);
    // to test SPI
    LSD_RF_WriteReg(REG_SYNCVALUE8, 0x55);
    if(LSD_RF_ReadReg(REG_SYNCVALUE8) != 0x55) 
    {
        return 1;// something wrong with SPI
    }
    LSD_RF_WriteReg(REG_SYNCVALUE8, 0xD3);
    if(LSD_RF_ReadReg(REG_SYNCVALUE8) != 0xD3) 
    {
        return 1;// something wrong with SPI
    }
   return 0;           
}

////////////////////////////////////////////////////////////////////////////
// �������� : ��������
// ������� : uint8_t *txBuffer:�������ݴ洢�����׵�ַ,
//            uint8_t size���������ݳ���
// ���ز��� : ��
// ˵��     : 
//////////////////////////////////////////////////////////////////////////////
void LSD_RF_SendPacket(uint8_t *txBuffer, uint8_t size)
{
    uint8_t i;
    unsigned long int j=600000;   //���ͳ�ʱ�˳�������ʵ��Ӧ����������
    LSD_RF_DIO0_IFG=0; //���MCU��־λ
    LSD_RF_DIO3_IFG=0; //���MCU��־λ
    LSD_RF_DIO0_IE=0;   //����ǰ�ر��ж�
    LSD_RF_DIO3_IE=0;   //����ǰ�ر��ж�
    LSD_RF_StandbyMode();               //����standbyģʽ
    LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAP1_DIO0_PKT_SENT); // DIO0 = packet sent
    #if (HIGH_POWER==1)   //���High Power ����enable��+20dbm capability on PA_BOOST��
    {
        LSD_RF_WriteReg(REG_OCP,0x0F);   
        LSD_RF_WriteReg(REG_TESTPA1,0x5D);   
        LSD_RF_WriteReg(REG_TESTPA2,0x7C);
    }
		#endif
    LSD_RF_ClearFIFO();    //���FIFO
    LSD_RF_WriteReg(REG_FIFO, size);
    for(i=0;i<size;i++)
    {
        LSD_RF_WriteReg(REG_FIFO, txBuffer[i]);
    }
    LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_TRANSMITTER));  //�л�������
    while((!LSD_RF_DIO0_IFG)&&j)j--;         // packet sent
    LSD_RF_StandbyMode();               //����standbyģʽ
    #if (HIGH_POWER==1)                //�˳����ͺ�ע��һ��Ҫ��Hing Power Disable��
    {
        LSD_RF_WriteReg(REG_OCP,0x1B);   
        LSD_RF_WriteReg(REG_TESTPA1,0x55);   
        LSD_RF_WriteReg(REG_TESTPA2,0x70); 
    }
    #endif
}

//////////////////////////////////////////////////////////////////////////////////
//// �������� : �������ݰ�
//// ������� : uint8_t *cRxBuf,�������ݴ洢�׵�ַ�� uint8_t cLength �������ݳ���
//// ���ز��� : ��
//// ˵��     : 
//////////////////////////////////////////////////////////////////////////////////
void LSD_RF_RxPacket(uint8_t *cRxBuf, uint8_t *cLength)
{
    uint8_t i;
    LSD_RF_StandbyMode();               //����standbyģʽ
    *cLength = LSD_RF_ReadReg(REG_FIFO);
    for(i=0;i<*cLength;i++)
      cRxBuf[i]=LSD_RF_ReadReg(REG_FIFO);
    LSD_RF_ClearFIFO();    //���FIFO
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : FreqCalSet
//// ������� : ch   ����ch=0~75
//// ���ز��� : ��
//// ˵��     : 
//////////////////////////////////////////////////////////////////////////////////
void LSD_RF_FreqSet(uint8_t ch)
{
    LSD_RF_StandbyMode();               //����standbyģʽ
    LSD_RF_WriteReg(REG_FRFMSB,Freq_Cal_Tab[ch*3]);
    LSD_RF_WriteReg(REG_FRFMID,Freq_Cal_Tab[ch*3+1]);
    LSD_RF_WriteReg(REG_FRFLSB,Freq_Cal_Tab[ch*3+2]);
}
////////////////////////////////////////////////////////////////////////////////
// �������� : WOR��ʼ��
// ������� : ��
//            
// ���ز��� : ��
// ˵��     : ǰ����2��+ͬ��(2)         
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WORInit(void)
{
    LSD_RF_StandbyMode();                //����standbyģʽ
    LSD_RF_DIO0_IFG=0;                   //���MCU��־λ
    LSD_RF_DIO0_IE=0;                    //�ر��ж�
    //���ò���
    LSD_RF_WriteReg(REG_PREAMBLELSB,0x02);   //����2��ǰ����
    LSD_RF_WriteReg(REG_SYNCCONFIG,0x88);   //����2��ͬ����
    LSD_RF_WriteReg(REG_SYNCVALUE1,0x55);   //����ͬ�������� 
    LSD_RF_WriteReg(REG_SYNCVALUE2,0x55);   //����ͬ�������� 
//    LSD_RF_WriteReg(REG_PACKETCONFIG1,0x80);   //�ر�CRC  
    LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO3_10); // ӳ��DIO3 = SYNC_ADDR��Ҳ������DIO0��������DIO3Ϊ���ӣ�    
    //��ʼ��ӳ���жϿ�
    LSD_RF_DIO3_IFG=0;   //����жϱ�־λ
    LSD_RF_DIO3_SEL=0;   //��ѡ��ڶ����ܣ�MSP430Ĭ��Ϊ��һ���ܣ�
    LSD_RF_DIO3_DIR=0;   //�ж�������
    LSD_RF_DIO3_IES=0;   //�������ж�        
}
////////////////////////////////////////////////////////////////////////////////
// �������� : ִ��WOR����
// ������� : uint8_t cLength  0 ����˯��ģʽ��  1 �������ģʽ��
//            
// ���ز��� : ��
// ˵��     :  DIO3��WOR�����ж�
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WORexecute(uint8_t cLength)
{
    unsigned long int i=600000; 
    switch(cLength)
    {
        case 0://����˯��ģʽ
                LSD_RF_DIO3_IE =0;      //�ر��ж�
//                Sleep_RX=0;             //ָʾ˯��
                LSD_RF_SleepMode();     //����˯��
          //      LSD_Sleep_Timerout();   //����˯�߶�ʱ��
                break;
        case 1://�������ģʽ
               LSD_RF_DIO3_IE =0;          //�ر��ж�
//               Sleep_RX=1;                 //ָʾ����
               LSD_RF_SleepMode();         //����˯�� 
               LSD_RF_ClearFIFO();         //��FIFO 
               LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_RECEIVER));//�������
               while (((LSD_RF_ReadReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00)&&i)i--; // Wait for ModeReady
               LSD_RF_DIO3_IFG=0;          //����жϱ�־λ
               LSD_RF_DIO3_IE =1;          //�����ж�
             //  LSD_RX_Timerout();          //�������ն�ʱ��
               break;
        default:break;
    }

}
////////////////////////////////////////////////////////////////////////////////
// �������� : �˳�WOR,������sdandbyģ��ʽ
// ������� : ��
// ���ز��� : ��
// ˵��     : �˳�WOR,���ı���ļĴ����ָ����Ĵ��������е�ֵ
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WOR_Exit(void)
{
   //�ر��ж�
    LSD_RF_DIO3_IFG=0;   //����жϱ�־λ
    LSD_RF_DIO3_IE =0;  //�ر��ж�
  //  TimerA_Init();      //���³�ʼ����ʱ�����رն�ʱ��
    //�ָ��Ĵ���
    LSD_RF_StandbyMode();                //����standbyģʽ
    LSD_RF_WriteReg(REG_DIOMAPPING1, 0x00); // �ָ�ԭֵ
    LSD_RF_WriteReg(REG_PREAMBLELSB,0x04);   //�ָ�ԭֵ
    LSD_RF_WriteReg(REG_SYNCCONFIG,0x9A);   //�ָ�ԭֵ
    LSD_RF_WriteReg(REG_SYNCVALUE1,0xD3);    //�ָ�ԭֵ
    LSD_RF_WriteReg(REG_SYNCVALUE2,0x91);    //�ָ�ԭֵ
//    LSD_RF_WriteReg(REG_PACKETCONFIG1,0x90);   //�ָ�ԭֵ   
}

////////////////////////////////////////////////////////////////////////////////
// �������� : ���Ѻ���
// ������� : ��
//            
// ���ز��� : ��
// ˵��     : ǰ�����ѷ�ʽ
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_AwakePkt(void)
{
    LSD_RF_DIO0_IFG=0;                   //���MCU��־λ
    LSD_RF_DIO0_IE=0;                    //�ر��ж�
    LSD_RF_StandbyMode();               //����standbyģʽ
    if(HIGH_POWER==1)   //���High Power ����enable��+20dbm capability on PA_BOOST��
    {
        LSD_RF_WriteReg(REG_OCP,0x0F);   
        LSD_RF_WriteReg(REG_TESTPA1,0x5D);   
        LSD_RF_WriteReg(REG_TESTPA2,0x7C);
    }
    LSD_RF_ClearFIFO();    //���FIFO
//     P6OUT |= BIT5;     //����̽��
    LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_TRANSMITTER));  //�л�������
    DelayMs(4400);    //ʵ��ʾ�������Է���ʱ��Ϊ4.2s
//     P6OUT &=~ BIT5;    //����̽��
    LSD_RF_StandbyMode();               //����standbyģʽ
    if(HIGH_POWER==1)                 //�˳����ͺ�ע��һ��Ҫ��Hing Power Disable��
    {
        LSD_RF_WriteReg(REG_OCP,0x1B);   
        LSD_RF_WriteReg(REG_TESTPA1,0x55);   
        LSD_RF_WriteReg(REG_TESTPA2,0x70); 
    }
}

////////////////////////////////////////////////////////////////////////////////////
////// �������� : RSSI���ܳ�ʼ��
////// ������� : ��
//////            
////// ���ز��� : ��
////// ˵��     : 
////////////////////////////////////////////////////////////////////////////////////
//void LSD_RF_RSSIInit(void)
//{
////     //��ʼ��ӳ���жϿ�
//    LSD_RF_DIO3_IFG=0;   //����жϱ�־λ
//    LSD_RF_DIO3_SEL=0;   //��ѡ��ڶ����ܣ�MSP430Ĭ��Ϊ��һ���ܣ�
//    LSD_RF_DIO3_DIR=0;   //�ж�������
//    LSD_RF_DIO3_IES=0;   //�������ж�        
//    LSD_RF_DIO3_IE =0;  //�ر��ж�
//    LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO3_10|RF_DIOMAP1_DIO0_PLD_RDY); // ӳ��DIO3 = SYNC_ADDR��Ҳ������DIO0��������DIO3Ϊ���ӣ�
//    LSD_RF_DIO3_IFG=0;          //����жϱ�־λ
//    LSD_RF_DIO3_IE =1;          //�����ж�   
//}

//////////////////////////////////////////////////////////////////////////////////
//// �������� : Trigger a RSSI Measurement ����һ��RSSI����
//// ������� : ��         
//// ���ز��� : ��
//// ˵��     : 
//////////////////////////////////////////////////////////////////////////////////
void LSD_RF_TriggerRssi(void)
{
    LSD_RF_WriteReg(REG_RSSICONFIG,0x01);   
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ����RSSIֵ
//// ������� : ��
////            
//// ���ز��� : ��
//// ˵��     : 
//////////////////////////////////////////////////////////////////////////////////
unsigned char LSD_RF_GetRSSI(void)
{
    unsigned char cRssi;
    cRssi = LSD_RF_ReadReg(REG_RSSIVALUE);
    cRssi = cRssi/2;
    return cRssi;
}
////////////////////////////////////////////////////////////////////////////////////
////// �������� : PreambleDetectorON
////// ������� : ��
//////            
////// ���ز��� : ��
////// ˵��     : ����Preamble��⹦��
////////////////////////////////////////////////////////////////////////////////////
//void LSD_RF_PreambleDetectorON(void)
//{  
//    //���ò���
//    LSD_RF_WriteReg(REG_DIOMAPPING2, 0x0F);
//    LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO3_01|RF_DIOMAP1_DIO0_PLD_RDY); 
//    LSD_RF_WriteReg(REG_PREAMBLE, 0xCA); // 
//
//    
//}
////////////////////////////////////////////////////////////////////////////////////
////// �������� : test
////// ������� : ��
//////            
////// ���ز��� : ��
////// ˵��     : 
////////////////////////////////////////////////////////////////////////////////////
//unsigned char cfei,cfei2;
//void LSD_RF_test(void)
//{
//    
//    cfei = LSD_RF_ReadReg(REG_AFCMSB);
//    cfei2 = LSD_RF_ReadReg(REG_AFCLSB);
//}






















































//////////////////////////////////////////////////////////////////////////////////
//// �������� : wor��ʼ��
//// ������� : ��
////            
//// ���ز��� : ��
//// ˵��     : ǰ����2��+ͬ��(1)         
//////////////////////////////////////////////////////////////////////////////////
//void LSD_RF_WORInit(void)
//{
//    LSD_RF_StandbyMode();                //����standbyģʽ
//    LSD_RF_DIO0_IFG=0;                   //���MCU��־λ
//    LSD_RF_DIO0_IE=0;                    //�ر��ж�
//    //����WOR����
//    LSD_RF_WriteReg(REG_LISTEN1,0xDA);   //����ListenResolldle =262ms��ListenResolRX =64us��RSSI&SyncAddress
//    LSD_RF_WriteReg(REG_LISTEN3,0x4E);   //���ý���ʱ�䣬T_rx=ListenCoefRX*ListenResolRX=0X4E x 64us=4.992ms
//    LSD_RF_WriteReg(REG_LISTEN2,0x0F);   //���÷���ʱ�䣬T_idle=ListenCoefIdle*ListenResolldle=0X0f x 262ms=3.930s
//    //
//    LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO3_10); // ӳ��DIO3 = SYNC_ADDR��Ҳ������DIO0��������DIO3Ϊ���ӣ�
//    LSD_RF_WriteReg(REG_RSSITHRESH,0xFA);   //����RSSI��ֵΪ-125dbm 
//    LSD_RF_WriteReg(REG_PREAMBLELSB,0x02);   //����2��ǰ����
//    LSD_RF_WriteReg(REG_SYNCCONFIG,0x82);   //����1��ͬ����
////    LSD_RF_WriteReg(REG_SYNCVALUE1,0x55);   //����1��ͬ����
//    
//    LSD_RF_WriteReg(REG_PACKETCONFIG1,0x80);   //�ر�CRC 
//      
//}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ִ��WOR����
//// ������� : void
////            
//// ���ز��� : ��
//// ˵��     :  DIO3��WOR�����ж�
//////////////////////////////////////////////////////////////////////////////////
//void LSD_RF_WORexecute(void)
//{
//    LSD_RF_StandbyMode();                //����standbyģʽ
//   //�����־λ
//    LSD_RF_DIO3_IFG=0;   //����жϱ�־λ
//    LSD_RF_DIO3_SEL=0;   //��ѡ��ڶ����ܣ�MSP430Ĭ��Ϊ��һ���ܣ�
//    LSD_RF_DIO3_DIR=0;   //�ж�������
//    LSD_RF_DIO3_IES=0;   //�������ж� 
//    LSD_RF_DIO3_IE =1;  //�����ж� 
//    LSD_RF_DIO3_IFG=0;   //����жϱ�־λ 
//    LSD_RF_WriteReg(REG_OPMODE,0x44);   //����LISTEN(WOR)
//}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : �˳�WOR,������sdandbyģ��ʽ
//// ������� : ��
//// ���ز��� : ��
//// ˵��     : �˳�WOR,���ı���ļĴ����ָ����Ĵ��������е�ֵ
//////////////////////////////////////////////////////////////////////////////////
//void LSD_RF_WOR_Exit(void)
//{
//   //�ر��ж�
//    LSD_RF_DIO3_IFG=0;   //����жϱ�־λ
//    LSD_RF_DIO3_IE =0;  //�ر��ж�
//    //�ָ��Ĵ���
//    LSD_RF_StandbyMode();                //����standbyģʽ
//    LSD_RF_WriteReg(REG_OPMODE,0x04);   //�ָ�ԭֵ
//    LSD_RF_WriteReg(REG_LISTEN1,0x92);   //�ָ�ԭֵ
//    LSD_RF_WriteReg(REG_LISTEN3,0x20);   //�ָ�ԭֵ
//    LSD_RF_WriteReg(REG_LISTEN2,0xF5);   //�ָ�ԭֵ
//    //
//    LSD_RF_WriteReg(REG_DIOMAPPING1, 0x00); // �ָ�ԭֵ
//    LSD_RF_WriteReg(REG_RSSITHRESH,0xE8);   //�ָ�ԭֵ 
//    LSD_RF_WriteReg(REG_PREAMBLELSB,0x04);   //�ָ�ԭֵ
//    LSD_RF_WriteReg(REG_SYNCCONFIG,0x9A);   //�ָ�ԭֵ
//    LSD_RF_WriteReg(REG_SYNCVALUE1,0xD3);    //�ָ�ԭֵ
//    LSD_RF_WriteReg(REG_PACKETCONFIG1,0x90);   //�ָ�ԭֵ   
//}
