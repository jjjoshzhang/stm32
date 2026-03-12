#include "stm32f10x.h"


void My_USART_Init(void);
void My_PC13LED_Init(void);

int main(void)
{
	
	My_USART_Init();
	My_PC13LED_Init();
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);

	
	while(1)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
		// read data from RDX
		uint8_t byteRcvd = USART_ReceiveData(USART1);
		
		if(byteRcvd == 0)
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET); // OFF
			}
		else if(byteRcvd == 1)
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET); //ON
			}
			
	}
}

void My_USART_Init(void)
{
	
	/*
	// PA9 tx
	
	GPIO_InitTypeDef GPIO_InitSturct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	
	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitSturct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitSturct);
	
	// PA10 rx

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitSturct);
	
	*/
	
	// use remap 
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	
	
		// PB6 tx
	
	GPIO_InitTypeDef GPIO_InitSturct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitSturct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitSturct);
	
	// PA10 rx

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitSturct);
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_Parity_No;
	USART_InitStruct.USART_WordLength = USART_StopBits_1;
	
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
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


