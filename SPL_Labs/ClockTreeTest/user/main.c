#include "stm32f10x.h"

// comment out SystemInit, config it's current 8Mz (main) clock tree to Max(System Init Setting)


void App_SystemClock_Init(void);
void My_PC13LED_Init(void);



int main(void)
{
	App_SystemClock_Init();
	My_PC13LED_Init();
	while(1)
	{
		// check CLK HZ
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		for(uint32_t i = 0; i<666666; i++);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
		for(uint32_t i = 0; i<666666; i++);
	}
}

void App_SystemClock_Init()
{
	
	// 0. ENABLE Flash's PreFetch Buffer 
	FLASH_PrefetchBufferCmd(ENABLE);
	FLASH_SetLatency(FLASH_Latency_2);
	// 1. RCC HSSE
	
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	
	// 2. Config and RCC PLL
	
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	// 3. Config AHP, APB1 and APB2's division ratio
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	
	// 4. Config SYSCLK source
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08); // 0x08 = from PLL
	
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


