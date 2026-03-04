
#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"


void My_LEDOnBoarding(void){
    // open the clock for GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
  // initialize PC13 as output OD and 2MHz
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
}


// initalize GPIO for USART1 a                                                                                                                                                                                          nd USART1 (default: PA9-tx and PA10-rx)
// Full duplex and 115200, 8bit, 1 stop bit, no-parity
void My_USART_Init(void){

// open GPIO for USART(DEFAULT)


// TX PA9(FROM  DATASHEET)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);


// RX PA10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);


// open USART

RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

USART_InitTypeDef USART_InitStruct = {0};
USART_InitStruct.USART_BaudRate = 115200;
USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;// both transmit and receive
USART_InitStruct.USART_WordLength = USART_WordLength_8b;
USART_InitStruct.USART_StopBits = USART_StopBits_1;
USART_InitStruct.USART_Parity = USART_Parity_No;


USART_Init(USART1, &USART_InitStruct);

USART_Cmd(USART1,ENABLE);
}



// print current time of stm32

int main(void){
    My_USART_Init();
    
    My_LEDOnBoarding();


    

for(;;)
{
    while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
    // assume bytes
    uint8_byteRcvd = USART_ReceiveData(USART1);
    if(byteRcvd == '0'){
    GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
    }else if(byteRcvd == '1'){
        GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
    }


}

}


