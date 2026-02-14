 #include "stm32f10x.h"


// init stm32 SI2C
void My_SI2C_Init(void);

void scl_write(uint8_t level);


int main(void)
{
    while(1)
    {
        My_SI2C_Init();
        


    }

}

void My_SI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOA,&GPIO_InitStruct);

}


void scl_write(uint8_t level)
{
    if(level == 0)
    {
        GPI_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET)
    }

}