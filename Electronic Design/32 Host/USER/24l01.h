//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01???????
#define L01_READ_REG        0x00  //??????,?5???????
#define L01_WRITE_REG       0x20  //??????,?5???????
#define RD_RX_PLOAD     0x61  //?RX????,1~32??
#define WR_TX_PLOAD     0xA0  //?TX????,1~32??
#define FLUSH_TX        0xE1  //??TX FIFO???.??????
#define FLUSH_RX        0xE2  //??RX FIFO???.??????
#define REUSE_TX_PL     0xE3  //?????????,CE??,????????.
#define NOP             0xFF  //???,??????????	 
//SPI(NRF24L01)?????
#define CONFIG          0x00  //???????;bit0:1????,0????;bit1:???;bit2:CRC??;bit3:CRC??;
//bit4:??MAX_RT(??????????)??;bit5:??TX_DS??;bit6:??RX_DR??
#define EN_AA           0x01  //????????  bit0~5,????0~5
#define EN_RXADDR       0x02  //??????,bit0~5,????0~5
#define SETUP_AW        0x03  //??????(??????):bit1,0:00,3??;01,4??;02,5??;
#define SETUP_RETR      0x04  //??????;bit3:0,???????;bit7:4,?????? 250*x+86us
#define RF_CH           0x05  //RF??,bit6:0,??????;
#define RF_SETUP        0x06  //RF???;bit3:????(0:1Mbps,1:2Mbps);bit2:1,????;bit0:????????
#define STATUS          0x07  //?????;bit0:TX FIFO???;bit3:1,???????(??:6);bit4,???????
//bit5:????????;bit6:??????;
#define MAX_TX  	0x10  //??????????
#define TX_OK   	0x20  //TX??????
#define RX_OK   	0x40  //???????

#define OBSERVE_TX      0x08  //???????,bit7:4,????????;bit3:0,?????
#define CD              0x09  //???????,bit0,????;
#define RX_ADDR_P0      0x0A  //????0????,????5???,?????
#define RX_ADDR_P1      0x0B  //????1????,????5???,?????
#define RX_ADDR_P2      0x0C  //????2????,???????,???,???RX_ADDR_P1[39:8]??;
#define RX_ADDR_P3      0x0D  //????3????,???????,???,???RX_ADDR_P1[39:8]??;
#define RX_ADDR_P4      0x0E  //????4????,???????,???,???RX_ADDR_P1[39:8]??;
#define RX_ADDR_P5      0x0F  //????5????,???????,???,???RX_ADDR_P1[39:8]??;
#define TX_ADDR         0x10  //????(?????),ShockBurstTM???,RX_ADDR_P0??????
#define RX_PW_P0        0x11  //??????0??????(1~32??),???0???
#define RX_PW_P1        0x12  //??????1??????(1~32??),???0???
#define RX_PW_P2        0x13  //??????2??????(1~32??),???0???
#define RX_PW_P3        0x14  //??????3??????(1~32??),???0???
#define RX_PW_P4        0x15  //??????4??????(1~32??),???0???
#define RX_PW_P5        0x16  //??????5??????(1~32??),???0???
#define FIFO_STATUS     0x17  //FIFO?????;bit0,RX FIFO??????;bit1,RX FIFO???;bit2,3,??
//bit4,TX FIFO???;bit5,TX FIFO???;bit6,1,?????????.0,???;
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TX_ADR_WIDTH    5   //5???????
#define RX_ADR_WIDTH    5   //5???????
#define TX_PLOAD_WIDTH  32  //20?????????
#define RX_PLOAD_WIDTH  32  //20?????????


void NRF24L01_Init(void);
void RX_Mode(void);
void TX_Mode(void);
u8 NRF24L01_Write_Buf(u8, u8 *, u8);
u8 NRF24L01_Read_Buf(u8, u8 *, u8);
u8 NRF24L01_Read_Reg(u8);
u8 NRF24L01_Write_Reg(u8, u8);
u8 NRF24L01_Check(void);
u8 NRF24L01_TxPacket(u8*);
u8 NRF24L01_RxPacket(u8*);


void NRF24L01_Init()
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//SCK=A5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//MISO=A6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//MOSI=A7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//SPI Conf
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
	//SPI-CONF

	//CS=C4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//INT=C5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//CE=A4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#define L01_CS GPIOC,GPIO_Pin_4
#define L01_INT GPIOC,GPIO_Pin_5
#define L01_CE GPIOA,GPIO_Pin_4
	GPIO_ResetBits(L01_CS);
	GPIO_ResetBits(L01_CE);
}

const u8 TX_ADDRESS[TX_ADR_WIDTH] = { 0x34, 0x43, 0x10, 0x10, 0x01 };
const u8 RX_ADDRESS[RX_ADR_WIDTH] = { 0x34, 0x43, 0x10, 0x10, 0x01 };



u8 NRF24L01_Check(void)
{
	u8 buf[5] = { 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	u8 i;
	NRF24L01_Write_Buf(L01_WRITE_REG + TX_ADDR, buf, 5);
	NRF24L01_Read_Buf(TX_ADDR, buf, 5);
	for (i = 0; i<5; i++)if (buf[i] != 0XA5)break;
	if (i != 5)return 1;
	return 0;
}


u8 SPIx_ReadWriteByte(u8 TxData)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, TxData);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}


u8 NRF24L01_Write_Reg(u8 reg, u8 value)
{
	u8 status;
	GPIO_ResetBits(L01_CS);
	status = SPIx_ReadWriteByte(reg);
	SPIx_ReadWriteByte(value);
	GPIO_SetBits(L01_CS);
	return(status);
}


u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;
	GPIO_ResetBits(L01_CS);
	SPIx_ReadWriteByte(reg);
	reg_val = SPIx_ReadWriteByte(0XFF);
	GPIO_SetBits(L01_CS);
	return(reg_val);
}


u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;
	GPIO_ResetBits(L01_CS);
	status = SPIx_ReadWriteByte(reg);
	for (u8_ctr = 0; u8_ctr<len; u8_ctr++)pBuf[u8_ctr] = SPIx_ReadWriteByte(0XFF);
	GPIO_SetBits(L01_CS);
	return status;
}




u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;
	GPIO_ResetBits(L01_CS);
	status = SPIx_ReadWriteByte(reg);
	for (u8_ctr = 0; u8_ctr<len; u8_ctr++)SPIx_ReadWriteByte(*pBuf++);
	GPIO_SetBits(L01_CS);
	return status;
}




u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	GPIO_ResetBits(L01_CE);
	NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);
	GPIO_SetBits(L01_CE);
	while (GPIO_ReadInputDataBit(L01_INT) != 0);
	sta = NRF24L01_Read_Reg(STATUS);
	NRF24L01_Write_Reg(L01_WRITE_REG + STATUS, sta);
	if (sta&MAX_TX)
	{
		NRF24L01_Write_Reg(FLUSH_TX, 0xff);
		return MAX_TX;
	}
	if (sta&TX_OK)
	{
		return TX_OK;
	}
	return 0xff;
}



u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;
	sta = NRF24L01_Read_Reg(STATUS);
	NRF24L01_Write_Reg(L01_WRITE_REG + STATUS, sta);
	if (sta&RX_OK)
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH);
		NRF24L01_Write_Reg(FLUSH_RX, 0xff);
		return 0;
	}
	return 1;
}

//const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};
//const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};

void RX_Mode(void)
{
	GPIO_ResetBits(L01_CE);
	NRF24L01_Write_Buf(L01_WRITE_REG + RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH);
	NRF24L01_Write_Reg(L01_WRITE_REG + EN_AA, 0x01);
	NRF24L01_Write_Reg(L01_WRITE_REG + EN_RXADDR, 0x01);
	NRF24L01_Write_Reg(L01_WRITE_REG + RF_CH, 40);
	NRF24L01_Write_Reg(L01_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
	NRF24L01_Write_Reg(L01_WRITE_REG + RF_SETUP, 0x0f);
	NRF24L01_Write_Reg(L01_WRITE_REG + CONFIG, 0x0f);
	GPIO_SetBits(L01_CE);
}


void TX_Mode(void)
{
	GPIO_ResetBits(L01_CE);
	NRF24L01_Write_Buf(L01_WRITE_REG + TX_ADDR, (u8*)TX_ADDRESS, TX_ADR_WIDTH);
	NRF24L01_Write_Buf(L01_WRITE_REG + RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH);
	NRF24L01_Write_Reg(L01_WRITE_REG + EN_AA, 0x01);
	NRF24L01_Write_Reg(L01_WRITE_REG + EN_RXADDR, 0x01);
	NRF24L01_Write_Reg(L01_WRITE_REG + SETUP_RETR, 0x1a);
	NRF24L01_Write_Reg(L01_WRITE_REG + RF_CH, 40);
	NRF24L01_Write_Reg(L01_WRITE_REG + RF_SETUP, 0x0f);
	NRF24L01_Write_Reg(L01_WRITE_REG + CONFIG, 0x0e);
	GPIO_SetBits(L01_CE);
}
