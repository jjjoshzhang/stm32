#include "stm32f10x.h"


void My_I2C_Init(void);
int My_I2C_SendBytes(I2C_TypeDef *I2Cx,uint8_t Addr, uint8_t *pData, uint16_t Size);
int My_I2C_ReceiveBytes(I2C_TypeDef *I2Cx,uint8_t Addr, uint8_t *pBuffer, uint16_t Size);
void My_PC13LED_Init(void);


int main(void)
{
	My_I2C_Init();
	My_PC13LED_Init();
	uint8_t commands[] = {0x00,0x8d,0x14,0xaf,0xa5};
	My_I2C_SendBytes(I2C1,0x78,commands,5);
	
	uint8_t rcvd;
	My_I2C_ReceiveBytes(I2C1,0x78,&rcvd,1);
	if((rcvd & (0x01 << 6)) == 0)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	}
	while(1)
	{
	}
}

void My_I2C_Init(void)
{
	// Remap
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
	
	// Init IO pins
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	// RESET
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,DISABLE);
	
	I2C_InitTypeDef I2C_InitStruct;
	
	// 400 khz
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_Init(I2C1,&I2C_InitStruct);
	
	I2C_Cmd(I2C1,ENABLE);
	
}

int My_I2C_SendBytes(I2C_TypeDef *I2Cx,uint8_t Addr, uint8_t *pData, uint16_t Size)
{
	// wait till bus idle
	while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY)== SET);
	
	// generate start bit 
	I2C_GenerateSTART(I2Cx,ENABLE);
	while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB)== RESET);
	
	// Addressing
	// clear AF
	I2C_ClearFlag(I2Cx,I2C_FLAG_AF);
	// last bit is 0
	I2C_SendData(I2Cx,Addr & 0xfe);
	
	while(1)
	{
		if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR)== SET) break;
		if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_AF) == SET)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return -1; // addressing went wrong
		}
		
	}
	// clear ADDR
	I2C_ReadRegister(I2Cx,I2C_Register_SR1);
	I2C_ReadRegister(I2Cx,I2C_Register_SR2);
	
	for(uint32_t i =0; i<Size; i++)
	{
		while(1)
		{
			if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_AF) == SET)
			{
				I2C_GenerateSTOP(I2Cx,ENABLE);
				return -2;
			}
			if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE) == SET)
			{
				break;
			}
		}
		I2C_SendData(I2Cx,pData[i]);
	}
		while(1)
		{
			if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_AF) == SET)
			{
				I2C_GenerateSTOP(I2Cx,ENABLE);
				return -2;
			}
			if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BTF) == SET) break;
		}
		
	I2C_GenerateSTOP(I2Cx,ENABLE);
		return 0; // send succesful 
}


int My_I2C_ReceiveBytes(I2C_TypeDef *I2Cx,uint8_t Addr, uint8_t *pBuffer, uint16_t Size)
{
	// Start Bit
	I2C_GenerateSTART(I2Cx,ENABLE);
	while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB)== RESET);
	
	// Addressing
	// Addressing
	// clear AF
	I2C_ClearFlag(I2Cx,I2C_FLAG_AF);
	// last bit is 0
	I2C_SendData(I2Cx,Addr | 0x01);
	
	while(1)
	{
		if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR)== SET) break;
		if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_AF) == SET)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return -1; // addressing went wrong
		}
		
	}
	// clear ADDR
	I2C_ReadRegister(I2Cx,I2C_Register_SR1);
	I2C_ReadRegister(I2Cx,I2C_Register_SR2);
	
	
	if(Size == 1)
	{
		// ACK = 0 STOP = 0
		I2C_AcknowledgeConfig(I2Cx,DISABLE);
		I2C_GenerateSTOP(I2Cx,ENABLE);
		
		while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_RXNE) == RESET);
		pBuffer[0] = I2C_ReceiveData(I2Cx);
	}
	else if(Size == 2)
	{
		//ACK = 1
		I2C_AcknowledgeConfig(I2Cx,ENABLE);
		
		while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_RXNE) == RESET);
		pBuffer[0] = I2C_ReceiveData(I2Cx);
		
		// ACK = 0 STOP = 0
		I2C_AcknowledgeConfig(I2Cx,DISABLE);
		I2C_GenerateSTOP(I2Cx,ENABLE);
		
		while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_RXNE) == RESET);
		pBuffer[1] = I2C_ReceiveData(I2Cx);
		
	}
	else
	{
		I2C_AcknowledgeConfig(I2Cx,ENABLE);
		for(uint32_t i =0; i<Size -1; i++)
		{
			while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_RXNE) == RESET);
			pBuffer[i] = I2C_ReceiveData(I2Cx);
		}
		// ACK = 0 STOP = 0
		I2C_AcknowledgeConfig(I2Cx,DISABLE);
		I2C_GenerateSTOP(I2Cx,ENABLE);
		
		while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_RXNE) == RESET);
		pBuffer[Size -1 ] = I2C_ReceiveData(I2Cx);
		
	}
	return 0;
}


void My_PC13LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitSturct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	
	
	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitSturct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitSturct);
	
	
}


