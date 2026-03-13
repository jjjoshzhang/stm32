/**
  ******************************************************************************
  * @file    adxl345.c
  * @author  jjjoshzhang
  * @version V 1.0.0
  * @date    3.12.2026
  * @brief   adxl345 source file
  ******************************************************************************
  */

#include "adxl345.h"
#include "delay.h"
#include "spi.h"
#include "stm32f10x.h"


void ADXL345_SPI_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	RCC_APB2PeriphClockCmd(ADXL345_PIN_CLK,ENABLE);

	GPIO_InitStruct.GPIO_Pin = ADXL345_SCK;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(ADXL345_PORT,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Pin = ADXL345_MISO;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(ADXL345_PORT,&GPIO_InitStruct);

	
	GPIO_InitStruct.GPIO_Pin = ADXL345_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(ADXL345_PORT,&GPIO_InitStruct);
	

	GPIO_InitStruct.GPIO_Pin = ADXL345_NSS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	
	GPIO_Init(ADXL345_PORT,&GPIO_InitStruct);
	
    // Set high
	GPIO_WriteBit(ADXL345_PORT,GPIO_Pin_15,Bit_SET);
	
	
	// SPI Init
	
	RCC_APB2PeriphClockCmd(ADXL345_SPI_CLK,ENABLE);
	
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(ADXL345_SPIPORT,&SPI_InitStruct);
	
    // set high
	SPI_NSSInternalSoftwareConfig(ADXL345_SPIPORT,SPI_NSSInternalSoft_Set);
	

} 