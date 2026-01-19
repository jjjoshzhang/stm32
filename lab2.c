
#include <stdint.h>
#include "stm32f10x.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{

  // open the clock for GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  // initialize PC13 as output OD and 2MHz
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_PU;

    GPIO_Init(GPIOA, &GPIO_InitStruct);

/* Loop forever */
	for(;;){
        // connected to ground and using IN_PU so when pressed reads 0 but led should lights
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1 == Bit_RESET)){
  GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
        }else{

  GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);

  }
}
