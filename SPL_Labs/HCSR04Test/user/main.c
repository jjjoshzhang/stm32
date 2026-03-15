#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

void APP_USART_Init(void);
void App_HCSR04_Init(void);

int main(void)
{
	APP_USART_Init();
	App_HCSR04_Init();

	while(1)
	{
		// clear cnt, cc1 and cc2 flags
		TIM_SetCounter(TIM1,0);
		TIM_ClearFlag(TIM1,TIM_FLAG_CC1);
		TIM_ClearFlag(TIM1,TIM_FLAG_CC2);
		
		// ENABLE TIM
		TIM_Cmd(TIM1, ENABLE);
		
		// send 10us pulse to pin Trig
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
		DelayUs(10);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
		
		// wait till cc1 and cc2 turn or 0 -> 1
		
		while(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC1) == RESET);
		while(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC2) == RESET);
		// Close TIM
		TIM_Cmd(TIM1, DISABLE);
		
		uint16_t ccr1 = TIM_GetCapture1(TIM1);
		uint16_t ccr2 = TIM_GetCapture2(TIM1);
		
		float distance = (ccr2-ccr1) * 1.0e-6f * 340.0f / 2.0f ;
		
		My_USART_Printf(USART1,"distance= %.4f\n",distance);
		
		Delay(5000);
		
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
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
}


void App_HCSR04_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	// PA8 CH1 IPU
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// config TIME BASE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

	TIM_ARRPreloadConfig(TIM1, ENABLE);

	
	
	
	// config Input Capture
	
	// DIRECT
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM1, &TIM_ICInitStruct);
	
	//INDIRECT
	
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInit(TIM1, &TIM_ICInitStruct);
	
	
	// INIT TRIG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	

	

	}


