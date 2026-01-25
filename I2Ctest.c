
#include "stm32f10x.h"

void My_I2C_Init(void);


int main(void){
    My_I2C_Init();



    

for(;;)
{

}

}

void My_I2C_Init(void){

    // use remap 

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);

    // PB8/PB9

    //init GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

  GPIO_Init(GPIOB,&GPIO_InitStruct);

  // init I2C

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,ENABLE);
  RCC_APB1PeriphResetkCmd(RCC_APB1Periph_I2C1,DISABLE);

  I2C_InitTypeDef I2C_InitStruct;
  I2C_InitStruct.I2C_ClockSpeed = 400000; // 400k for OLED
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;


  I2C_Init(I2C1,&I2C_InitStruct);
  I2C_Cmd(I2C1,ENABLE);


  













}