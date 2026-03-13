#include "stm32f10x.h"


void APP_SPI_Init(void);
void APP_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx,
																	const uint8_t *pDataTx, uint8_t *pDataRx,uint16_t Size);
void APP_W25Q64_SaveByte(uint8_t byte);
uint8_t APP_W25Q64_LoadByte(void);

uint8_t a;

int main(void)
{
	APP_SPI_Init();
	APP_W25Q64_SaveByte(0x12);
	a = APP_W25Q64_LoadByte();
	
	while(1)
	{
	}
}


void APP_SPI_Init(void)
{
	// REMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	// PB3 SCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	// PB4 MISO
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//PB5 MOSI
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	// PB15 General IO simulate SC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
	
	
	// SPI Init
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI1,&SPI_InitStruct);
	
	SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Set);
	
}

void APP_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx,const uint8_t *pDataTx, uint8_t *pDataRx,uint16_t Size)
{
	SPI_Cmd(SPIx,ENABLE);
	
	SPI_I2S_SendData(SPIx,pDataTx[0]);
	
	for(uint32_t i =0; i<Size-1; i++)
	{
		
		// receive 1 byte
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE)== RESET);
		pDataRx[i] = SPI_I2S_ReceiveData(SPIx);

		// send 1 byte
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE)== RESET);
		SPI_I2S_SendData(SPIx,pDataTx[i+1]);
		
	}
	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE)== RESET);
	pDataRx[Size-1] = SPI_I2S_ReceiveData(SPIx);
	SPI_Cmd(SPIx,DISABLE);

}

void APP_W25Q64_SaveByte(uint8_t byte)
{
	uint8_t buffer[10];
	// 1. Write Enable
	// Enable Commands
	buffer[0] = 0x06;
	// choose W25Q64
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
	APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,1);
	// Close
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
	
	// 2. Sector Erase
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
	// Erase Commands
	buffer[0] = 0x20;
	// 24-bit address
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,4);
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
	
	// 3. Wait till Busy 1 -> 0 
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
		// Send Read Register Commands
		buffer[0] = 0x05;
		APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,1);
		// Read Register 1
		buffer[0] = 0xff;
		APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,1);
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
		
		if((buffer[0] & 0x01) == 0) break;

	}
	// 4. Write Enable
	// Enable Commands
	buffer[0] = 0x06;
	// choose W25Q64
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
	APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,1);
	// Close
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
	
	// 5. Page program
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
	// Program Commands
	buffer[0] = 0x02;
	// 24-bit address
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	// actual Data
	buffer[4] = byte;
	APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,5);
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
	
	// 6. Wait till Busy 1 -> 0 
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
		// Send Read Register Commands
		buffer[0] = 0x05;
		APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,1);
		// Read Register 1
		buffer[0] = 0xff;
		APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,1);
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
		
		if((buffer[0] & 0x01) == 0) break;

	}
	

}

uint8_t APP_W25Q64_LoadByte(void)
{
	uint8_t buffer[10];
	// Read Data
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
	// Read Data Commands
	buffer[0] = 0x03;
	// 24-bit address
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,4);
	
	buffer[0] = 0xff;
	APP_SPI_MasterTransmitReceive(SPI1,buffer, buffer,1);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
	
	return buffer[0];
	
}


