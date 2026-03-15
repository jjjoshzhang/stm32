#include "stm32f10x.h"

void APP_ADC1_Init(void);

void My_PC13LED_Init(void);

int main(void)
{
	APP_ADC1_Init();
	My_PC13LED_Init();
	while(1)
	{
		// Clear EOC flag
		ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
		// Send a pulse to start
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		// wait while Conversion is done
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
		// read conversion value 
		uint16_t DR = ADC_GetConversionValue(ADC1);
		// get voltage
		
		float voltage = DR*(3.3f / 4095.0f);
		
		// > 1.5 = lux is too low 
		if(voltage > 1.5)
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_SET); // ON
		}else{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_RESET); // OFF
		}
		
		
	}
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
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // enable by software
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	
	// Config Regular Channel
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1, ADC_SampleTime_13Cycles5); // 10.24 cycle
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);
	
	ADC_Cmd(ADC1,ENABLE);
	
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
