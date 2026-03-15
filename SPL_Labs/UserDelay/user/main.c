#include "stm32f10x.h"


volatile uint32_t currentTick = 0; // record current time in ms
void APP_Delay(uint32_t ms);
void APP_TIM3_TimeBaseInit(void);
void My_PC13LED_Init(void);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	APP_TIM3_TimeBaseInit();
	My_PC13LED_Init();
	while(1)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		APP_Delay(50);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
		APP_Delay(500);
		
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		APP_Delay(50);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
		APP_Delay(500);
		
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		APP_Delay(500);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
		APP_Delay(500);
		
		
	}
}


void APP_Delay(uint32_t ms)
{
	uint32_t expireTime = currentTick + ms;
	
	while(currentTick < expireTime);
}

void APP_TIM3_TimeBaseInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	// config TIME BASE
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_Cmd(TIM3,ENABLE);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);
	
	
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==SET)
	{
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		currentTick ++;
	}
	
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

