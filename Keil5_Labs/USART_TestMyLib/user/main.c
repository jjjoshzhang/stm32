#include "stm32f10x.h"
#include "usart.h"
void My_USART_Init(void);
int main(void)
{
	My_USART_Init();
	
	// My_USART_SendByte(USART1,0x5a);
	
	/*
	uint8_t byteArray[] = {1,2,3,4,5};
	My_USART_SendBytes(USART1,byteArray,5);
	*/
	
	// My_USART_SendChar(USART1,'A');
	
	
	// My_USART_SendString(USART1,"Hello World");
	
	
	
	
	while(1)
	{
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
