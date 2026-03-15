#include "stm32f10x.h"

void My_PC13LED_Init(void);
void APP_Button_Init(void);
void APP_EXTI_Init(void);


int main(void)
{
	// choose NVIC Group
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	My_PC13LED_Init();
	APP_Button_Init();
	APP_EXTI_Init();
	while(1)
	{
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
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
}


void APP_Button_Init(void)
{
	GPIO_InitTypeDef GPIO_InitSturct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	
	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(GPIOA,&GPIO_InitSturct);
	
}
	
void APP_EXTI_Init(void)
{
	// need AFIO for EXTI routing(and REMAP)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	
	// choose ports for 2 buttons
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStruct);
	
	// Config Interupt
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
		
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line0) == SET)
	{
		EXTI_ClearFlag(EXTI_Line0);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET); // OFF
	}
	
	
}
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line1) == SET)
	{
		EXTI_ClearFlag(EXTI_Line1);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET); // ON
	}
	
	
}




