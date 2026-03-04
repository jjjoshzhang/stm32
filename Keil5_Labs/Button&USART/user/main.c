#include "stm32f10x.h"
#include "button.h"
#include "usart.h"

Button_TypeDef button;

uint32_t count = 0;

void APP_Button_Init(void);
void APP_USART_Init(void);

void button_clicked_cb(uint8_t clicks);
void button_long_pressed_cb(uint8_t ticks);



int main(void)
{
	APP_USART_Init();
	APP_Button_Init();
	My_USART_SendString(USART1,"HELLO WORLD");
	
	while(1)
	{
		My_Button_Proc(&button);
	}
}



void APP_USART_Init(void)
{
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

void APP_Button_Init(void)
{
	Button_InitTypeDef Button_InitStruct;
	Button_InitStruct.GPIOx = GPIOA;
	Button_InitStruct.GPIO_Pin = GPIO_Pin_0;
	Button_InitStruct.ClickInterval = 0;
	Button_InitStruct.LongPressTickInterval = 0;
	Button_InitStruct.LongPressTime = 0;
	Button_InitStruct.button_clicked_cb = button_clicked_cb;
	Button_InitStruct.button_long_pressed_cb = button_long_pressed_cb;
	Button_InitStruct.button_pressed_cb = 0;
	Button_InitStruct.button_released_cb = 0;
	My_Button_Init(&button,&Button_InitStruct);
	
}

void button_clicked_cb(uint8_t clicks)
{
	if(clicks == 1)
	{
		count ++;
		My_USART_Printf(USART1,"%d",count);
	}else if(clicks == 2)
	{
		count = 0;
		My_USART_Printf(USART1,"%d",count);
	}
}
void button_long_pressed_cb(uint8_t ticks)
{
	count ++;
	My_USART_Printf(USART1,"%d",count);
}
