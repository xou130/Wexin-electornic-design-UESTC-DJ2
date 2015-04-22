#include "stm32f10x.h"
#include "sys.h"
#include "LCD.h"
//#include "I2C.h"
#include "stdlib.h"

#define uchar unsigned char
#define uint unsigned int

/*sys:delayms() not precise*/
int time=10;
void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			 //??LED?? D2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
}	

void Timer_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_DeInit(TIM3);

	TIM_TimeBaseStructure.TIM_Period=2000;
	TIM_TimeBaseStructure.TIM_Prescaler=35999;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
}
























#define TX_ADR_WIDTH    5   // 5  bytes TX(RX) address width

#define TX_PLOAD_WIDTH  32  // 32 bytes TX payload


#define L01_READ_REG        0x00  // Define read command to register
#define L01_WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define STA_MARK_RX     0X40
#define STA_MARK_TX     0X20
#define STA_MARK_MX     0X10	
u8 TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address
u8 RX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01};

void SPI_24L01_SPI_Init()
{
SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	//SCK=A5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//MISO=A6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//MOSI=A7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//SPI Conf
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
	//SPI-CONF
	
	//CS=C4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//INT=C5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//CE=A4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#define L01_CS GPIOC,GPIO_Pin_4
#define L01_INT GPIOC,GPIO_Pin_5
#define L01_CE GPIOA,GPIO_Pin_4
}

u8 L01_SPI_RW(u8 da)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, da);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

u8 SPI_RW_Reg(u8 reg, u8 value)
{
	uchar status;
  	GPIO_ResetBits(L01_CS);                   // CSN low, init SPI transaction
  	status = L01_SPI_RW(reg);      // select register
  	L01_SPI_RW(value);             // ..and write value to it..
  	GPIO_SetBits(L01_CS);                  // CSN high again
  	return(status);            // return nRF24L01 status byte
}

u8 SPI_Read(u8 reg)
{
	u8 reg_val;
  	GPIO_ResetBits(L01_CS);                // CSN low, initialize SPI communication...
  	L01_SPI_RW(reg);            // Select register to read from..
  	reg_val = L01_SPI_RW(0);    // ..then read registervalue
  	GPIO_SetBits(L01_CS);               // CSN high, terminate SPI communication
  	return(reg_val);        // return register value
}

u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 bytes)
{
	uchar status,byte_ctr;

GPIO_ResetBits(L01_CS);                    		// Set CSN low, init SPI tranaction
  	status = L01_SPI_RW(reg);       		// Select register to write to and read status byte

  	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    	pBuf[byte_ctr] = L01_SPI_RW(0);    // Perform SPI_RW to read byte from nRF24L01

  	GPIO_SetBits(L01_CS);                          // Set CSN high again

  	return(status);                    // return nRF24L01 status byte
}
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 bytes)
{
	uchar status,byte_ctr;

  	GPIO_ResetBits(L01_CS);                  // Set CSN low, init SPI tranaction
  	status = L01_SPI_RW(reg);    // Select register to write to and read status byte
  	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
    	L01_SPI_RW(*pBuf++);
  	GPIO_SetBits(L01_CS);                // Set CSN high again
  	return(status);          // return nRF24L01 status byte
}

void receive_mode_init(void) 
{
	GPIO_ResetBits(L01_CE);
  	SPI_Write_Buf(L01_WRITE_REG + RX_ADDR_P0,RX_ADDRESS, TX_ADR_WIDTH); // Use the same address on the RX device as the TX device
  	SPI_RW_Reg(L01_WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(L01_WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  	SPI_RW_Reg(L01_WRITE_REG + RF_CH, 40);        // Select RF channel 40
  	SPI_RW_Reg(L01_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
  	SPI_RW_Reg(L01_WRITE_REG + RF_SETUP, 0x27);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg(L01_WRITE_REG + CONFIG, 0x0f);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
    SPI_RW_Reg(L01_WRITE_REG+STATUS,0xff); // ????????
  	GPIO_SetBits(L01_CE); // Set CE pin high to enable RX device 
}

void SPI_CLR_Reg(u8 R_T)
{
  	GPIO_ResetBits(L01_CS); 
	if(R_T==1)                  // CSN low, init SPI transaction
  	L01_SPI_RW(FLUSH_TX);             // ..and write value to it..
	else
	L01_SPI_RW(FLUSH_RX);             // ..and write value to it..
  GPIO_SetBits(L01_CS);                  // CSN high again
}

void ifnnrf_CLERN_ALL()
{
  SPI_CLR_Reg(0);
  SPI_CLR_Reg(1);
  SPI_RW_Reg(L01_WRITE_REG+STATUS,0xff);
  GPIO_SetBits(L01_INT);
}
u8 receive_data(u8 *buf)
{
    uchar RXflag = 0;
    GPIO_SetBits(L01_INT);
    if (GPIO_ReadInputDataBit(L01_INT)==0) {  //!!!!How to read int here?
        if (SPI_Read(STATUS) & STA_MARK_RX) {
            SPI_Read_Buf(RD_RX_PLOAD,buf,TX_PLOAD_WIDTH);
            RXflag = 1;
            SPI_RW_Reg(L01_WRITE_REG+STATUS,0xff); //Clear all interrupt flages
        } else {
            ifnnrf_CLERN_ALL();
            RXflag = 0;
        }
    }    
    return RXflag;  
}




















int main(void)
{int K1;int rx;
	Timer_Init();
	LED_Init();
LCD_SPI_Init();
LCD_fInit();
LCD_Clear(0xFFFF);
LCD_ShowString(0,0,"Test of Timer & 24L01");
	GPIO_SetBits(GPIOG,GPIO_Pin_14);
	SPI_24L01_SPI_Init();
	receive_mode_init();
	L01_SPI_RW(0);
	GPIO_ResetBits(GPIOG,GPIO_Pin_14);
	LCD_ShowString(0,100,"24L01 Send Data Success! Read Data Following:");
	
	LCD_ShowNum(200,116,rx,3);
while(1){
K1=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0);
	if(K1)GPIO_ResetBits(GPIOG,GPIO_Pin_14);
	else GPIO_SetBits(GPIOG,GPIO_Pin_14);
}

}



void TIM3_IRQHandler(void)//enter every 1 second
{
TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
LCD_ShowNum(80,144,time,5);
time--;
	if(time<0){
		BACK_COLOR=RED;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,DISABLE);
		LCD_Clear(RED);
		LCD_ShowString(90,144,"Boom!");
		LCD_ShowString(40,230,"\\(^o^)/");
		LCD_ShowString(120,246,"\\(^o^)/");
		LCD_ShowString(50,20,"\\(^o^)/");
		LCD_ShowString(170,36,"\\(^o^)/");
		LCD_ShowChar(100,100,'*',1);
		LCD_ShowChar(100,190,'*',1);
		LCD_ShowChar(50,144,'*',1);
		LCD_ShowChar(160,144,'*',1);
}

}

