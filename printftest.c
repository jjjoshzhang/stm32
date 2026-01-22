
#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"


/*
    use USART to send mutliple bytes of data that stores in an array called pData
void My_USART_sendBytes(USART_TypeDef* USARTx,uint8_t *pData, uint16_t size){

    for(uint_32 i =0; i<size; i++){
        wait till transmit register is empty
        while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)== RESET);
            USART_SendData(USART1,pData[i]);
    }
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)== RESET);

}
*/



// initalize GPIO for USART1 and USART1 (default: PA9-tx and PA10-rx)
// Full duplex and 115200, 8bit, 1 stop bit, no-parity
void My_USER_Init(void){

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

int fputc(int ch, FILE* f){
    while(USART_GetFlagsStatus(USART1,USART_FLAG_TXE== RESET));
    USART_SendData(USART1,(uint8_t)ch);// type cast
    return ch;

}



// print current time of stm32

int main(void){
    Delay_Init();
    My_USART_Init();

    

for(;;)
{
    uint32_t currentTick = GetTick();

    uint32_t ms = currentTick % 1000;
    currentTick /= 1000;
    uint32_t s = currentTick % 60;
    currentTick /= 60;
    uint32_t m = currentTick % 60;
    current /= 60;
    uint32_t h = currentTick;

    printf("%02u:%02u:%02u.%03u\r\n", h,m,s,ms);
    Delay(100);
}

}


