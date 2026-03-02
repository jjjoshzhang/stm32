
#include "stm32f10x.h"


// init stm32 I2C
void My_I2C_Init(void);

// send data from stm32 to slave devices
int My_I2C_SendBytes(I2C_TypeDef *I2Cx, 
  uint8_t Addr, // address
  uint8_t *pData, 
  uint16_t Size);


// return 0 means read sucessfully, -1 means addressing went wrong
int My_I2C_ReceiveBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t *pBuffer, uint16_t Size);




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

// will return an int, 0 means success, -1 means cant find the addr, -2 means sent data got declined
int My_I2C_SendBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint16_t Size){

  // wait till I2C bus becomes empty 
  // 0 means free, 1 means busy
  while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY)== SET);

  // set start bit to 1  
  I2C_GenerateStart(I2Cx,ENABLE);
  // wait till start bit get sent, 0->1
  while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB) == RESET);
  
  //addressing 

  // reset AF flag
  I2C_ClearFlag(I2Cx,I2C_FlAG_AF);
  // set R/W = 0
  // we are sending addr now 
  I2C_SendData(I2Cx,Addr & 0xfe);

// infinite loop bc the code executes faster than I2C hardware
  for(;;){
    if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR) == SET){
      // addressing succesfully 
      break;
    } 
    if(I2C_GetFlagStatus(I2Cx,I2C_FlAG_AF)== SET){  
      I2C_GenerateSTOP(I2Cx,ENABLE); // send stop bit 
      
      return -1; // addressing went wrong, return -1
    }
  }


  // clear addr flag
  // clear it here bc we know that addressing is successful
  I2C_ReadRegister(I2Cx,I2C_Register_SR1);
  I2C_ReadRegister(I2Cx,I2C_Register_SR2);

// send data
  for(uint16_t i =0; i<SIZE; i++)
  {
    while(1)
    {  
      if(I2C_GetFlagStatus(I2Cx,I2C_FlAG_AF) == SET)
{
  // last data was rejected (NACKed)
  I2C_GenerateSTOP(I2Cx,ENABLE);
  return -2;
}    
// register is empty
if(I2C_GetFlagStatus(I2Cx,I2C_FlAG_TXE) == SET)
{
  break;
} 
}
I2C_SendData(I2Cx,pData[i]);
  }

  while(1)
  {
    if(I2C_GetFlagStatus(I2Cx,I2C_FlAG_AF) == SET)
{
  // last data was rejected (NACKed)
  I2C_GenerateSTOP(I2Cx,ENABLE);
  return -2;
} 
    if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BTF)==SET)
    {
      break;
    }
  }

  I2C_GenerateStop(I2Cx,ENABLE);
  return 0; // data was sent sucessfully 


}




int My_I2C_ReceiveBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t *pBuffer, uint16_t Size)
{ 
  // set start bit to 1  
  I2C_GenerateStart(I2Cx,ENABLE);

  while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB) == RESET);

  // addressing 

  // reset AF flag
  I2C_ClearFlag(I2Cx,I2C_FlAG_AF);
  // make R/W = 1
  I2C_SendData(I2Cx, Addr | 0x01);


  while(1)
  {
    if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_AF) == SET)
    {
      I2C_GenerateSTOP(I2Cx,ENABLE);
      return -1; // addressing went wrong 
    }
    if(I2C_GetFlagStatus(I2Cx,I2C_FlAG_TXE) == SET)
    {
      break;
    } 
  }

  // receive data 

// 1 byte
  if(Size == 1)
  {
    // clear addr flag
  
    I2C_ReadRegister(I2Cx,I2C_Register_SR1);
    I2C_ReadRegister(I2Cx,I2C_Register_SR2);

    // set ACK to 0, STOP = 1
    // "pre-program"
    I2C_AcknowledgeConfig(I2Cx,DISABLE);

    I2C_GenerateStop(I2Cx,ENABLE);

    while(I2C_GetFlagStatus(I2Cx,I2C_FlAG_RXNE) == RESET);

    pBuffer[0] = I2C_ReceiveData(I2Cx);

  }else if(Size == 2)
  {
    // clear addr flag
  
    I2C_ReadRegister(I2Cx,I2C_Register_SR1);
    I2C_ReadRegister(I2Cx,I2C_Register_SR2);

    // set ACK to 1
    I2C_AcknowledgeConfig(I2Cx,ENABLE);

    // wait till the first data is ready to read

    while(I2C_GetFlagStatus(I2Cx,I2C_FlAG_RXNE) == RESET);

    pBuffer[0] = I2C_ReceiveData(I2Cx);


     // set ACK to 0, STOP = 1 (for second data)
    // "pre-program"
    I2C_AcknowledgeConfig(I2Cx,DISABLE);

    I2C_GenerateStop(I2Cx,ENABLE);

    while(I2C_GetFlagStatus(I2Cx,I2C_FlAG_RXNE) == RESET);
    // receive second data 
    pBuffer[1] = I2C_ReceiveData(I2Cx);


  }else
  {
    // clear addr flag
  
    I2C_ReadRegister(I2Cx,I2C_Register_SR1);
    I2C_ReadRegister(I2Cx,I2C_Register_SR2);

    // set ACK to 1
    I2C_AcknowledgeConfig(I2Cx,ENABLE);

    for(uint16_t i =0, i<Size-1, i++)
    {


      // wait till the first data is ready to read

      while(I2C_GetFlagStatus(I2Cx,I2C_FlAG_RXNE) == RESET);

      pBuffer[i] = I2C_ReceiveData(I2Cx);

    }


    I2C_AcknowledgeConfig(I2Cx,DISABLE);

    I2C_GenerateStop(I2Cx,ENABLE);

    while(I2C_GetFlagStatus(I2Cx,I2C_FlAG_RXNE) == RESET);
 
    pBuffer[Size-1] = I2C_ReceiveData(I2Cx);

  }n 


  return 0;

 
}