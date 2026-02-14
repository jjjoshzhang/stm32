 #include "stm32f10x.h"


// init stm32 SI2C
void My_SI2C_Init(void);

void scl_write(uint8_t level);
void sda_write(uint8_t level);
uint8_t sda_read(void);
void delay_us(uint32_t us);

void SendStart(void);
void SendStop(void);


int main(void)
{
    while(1)
    {
        My_SI2C_Init();
        


    }

}

void My_SI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOA,&GPIO_InitStruct);

}


void scl_write(uint8_t level)
{
    // PA0
    if(level == 0)
    { 
        GPI_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
    }
    else
    {
        GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET)
    }

}
 

void sda_write(uint8_t level)
{
    // PA1
    if(level == 0)
    { 
        GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
    }
    else
    {
        GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET)
    }

}
uint8_t sda_read(void)
{
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == Bit_SET)
    {
        return 1;
    }
    else
    {
        return 0;

    }
}
void delay_us(uint32_t us){
    // abt 8 loop iteration = 1us
    uint32_t n = us * 8;
    for(uint32_t i =0; i<n; i++);
}


void SendStart(void)
{
    sda_write(0);
    delay_us(1); 
}


