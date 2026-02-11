 #include "stm32f10x.h"


// init stm32 SI2C
void My_SI2C_Init(void);


int main(void)
{
    while(1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO, ENABLE);
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
        


    }

}

void My_SI2C_Init(void)
{
    RCC

}