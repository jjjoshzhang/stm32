#include "stm32f10x.h"
#include "usart.h"
void APP_USART_Init(void);
void APP_TIM1_Init(void);
void APP_ADC1_Init(void);

int main(void)
{
	APP_USART_Init();
	APP_TIM1_Init();
	APP_ADC1_Init();

	while(1)
	{

		// wait till it's done
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC) == RESET);		
		ADC_ClearFlag(ADC1,ADC_FLAG_JEOC);
		uint16_t jdr1 = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1);
		float voltage = jdr1 *(3.3f/4095.0f);
		
		// send voltage through USART
		My_USART_Printf(USART1,"%.3f\n", voltage);
		
		
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
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_Parity_No;
	USART_InitStruct.USART_WordLength = USART_StopBits_1;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
}



void APP_TIM1_Init(void)
{
	// TIM1
	
	// config TIME BASE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	// Enable Preload 
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	// set TRGO to update mode
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);
	TIM_Cmd(TIM1, ENABLE);

}



void APP_ADC1_Init(void)
{
	// Init GPIOPin PA0
	GPIO_InitTypeDef GPIO_InitSturct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitSturct);
	
	// Init ADC 
	// Enable ADC Clock and make sure it is under 14MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	
	ADC_InitTypeDef ADC_InitStruct = {0};
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // for regular sequence 
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	
	// Config Injected Sequence Channel
	
	ADC_InjectedSequencerLengthConfig(ADC1,1);
	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_T1_TRGO);
	ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_13Cycles5); // 10.24 cycles
	
	ADC_Cmd(ADC1,ENABLE);
	
}
