#include "stm32f10x.h"
#include "usart.h"
#include "math.h"
#include "delay.h"

void APP_USART_Init(void);
void App_PWM_Init(void);
void APP_Measure_Init(void);


int main(void)
{
	APP_USART_Init();
	App_PWM_Init();
	APP_Measure_Init();
	TIM_SetCompare1(TIM3,200);

	while(1)
	{
		// clear Trigger Flag
		TIM_ClearFlag(TIM1,TIM_FLAG_Trigger);
		

		
		while(TIM_GetFlagStatus(TIM1,TIM_FLAG_Trigger) == RESET);
		

		uint16_t ccr1 = TIM_GetCapture1(TIM1);
		uint16_t ccr2 = TIM_GetCapture2(TIM1);
		
		float period = ccr1*1e-3f; // in ms
		float duty = ((float)ccr2 / ccr1) *100.0f;
		
		My_USART_Printf(USART1,"Period = %.3fms, Duty = %.2f%%",period, duty);
		Delay(100);
		
		

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

	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef USART_InitStruct = {0};
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_Parity_No;
	USART_InitStruct.USART_WordLength = USART_StopBits_1;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
}



void App_PWM_Init(void)
{
	// TIM3
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	// PA6 CH1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	
	// config TIME BASE
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	// Enable Preload 
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_Cmd(TIM3, ENABLE);
	
	// config Output Compare
	
	TIM_OCInitTypeDef TIM_OCInitStruct = {0};
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	
	// ENABLE MOE
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	
	// Enable Preload 
	TIM_CCPreloadControl(TIM3, ENABLE);
	}

void APP_Measure_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
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
	// TIM_Cmd(TIM1, ENABLE);
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
	
	// Slave Mode Control
	TIM_SelectInputTrigger(TIM1,TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);
}

