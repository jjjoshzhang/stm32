#include "stm32f10x.h"
#include "delay.h"

void My_PC13LED_Init(void);
void AMP_Button_Init(void);


int main(void)
{
	My_PC13LED_Init();
	AMP_Button_Init();
	
	uint8_t previous, current = Bit_SET;
	
	while(1)
	{
		previous = current;
		current = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
		if(current != previous)
		{
			Delay(10);

			if(current == Bit_SET)
			{
				if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13) == Bit_SET)
				{
					GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
				}
				else
				{
					GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
				}
			}

			
		}

		
	}
}

void AMP_Button_Init(void)
{
	GPIO_InitTypeDef GPIO_InitSturct = {0};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(GPIOA,&GPIO_InitSturct);
	
}

void My_PC13LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitSturct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitSturct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitSturct);
}
