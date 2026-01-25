
#include "stm32f10x.h"


// init stm32 I2C
void My_I2C_Init(void);

// send data from stm32 to slave devices
int My_I2C_SendBytes(I2C_TypeDef *I2Cx, 
  uint8_t Addr, // adress
  uint8_t *pData, 
  uint16_t Size);


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


int My_I2C_SendBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint16_t Size){
  // wait till I2C bus becomes empty 
  while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY)== SET);

  // START 
  I2C_GenerateStart(I2Cx,ENABLE);
  // wait till it's enabled, 0->1
  while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB) == RESET);
  
  //addressing 
  // reset AF flag
  I2C_ClearFlag(I2Cx,I2C_FlAG_AF);
  // & 0xfe make it shift left 
  I2C_SendData(I2Cx,Addr & 0xfe);

  for(;;){
    if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR) == SET){
      // addressing succesfully 
      break;
    } 
    if(I2C_GetFlagStatus(I2Cx,I2C_FlAG_AF)== SET){
      I2C_GenerateSTOP(I2Cx,ENABLE);
      // addressing went wrong
      return -1;
    }
  }

  // clear addr flag
  I2C_ReadRegister(I2Cx,I2C_Register_SR1);
  I2C_ReadRegister(I2Cx,I2C_Register_SR2);
// send data
  for(uint16_t i =0; i<SIZE; i++){
    while(1){
      if(I2C_GetFlagStatus(I2Cx,I2C_FlAG_AF)== SET)
{
  I2C_GenerateSTOP(I2Cx,ENABLE);
}    
if(I2C_GetFlagStatus(I2Cx,I2C_FlAG_TXE)== SET){
  break;
} 
}
    

  }


}