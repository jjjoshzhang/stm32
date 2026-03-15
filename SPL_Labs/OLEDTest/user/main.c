#include "stm32f10x.h"
#include "si2c.h"
#include "oled.h"
#include "hyjk16.h"
#include "delay.h"

SI2C_TypeDef si2c;
OLED_TypeDef oled;


void My_SoftwareI2C_Init(void);
void My_OLEDScreen_Init(void);
int i2c_write_bytes(uint8_t addr, const uint8_t *pdata, uint16_t size);

int main(void)
{
	My_SoftwareI2C_Init();
	My_OLEDScreen_Init();
	
//	// Hello World
//	OLED_SetPen(&oled,PEN_COLOR_WHITE,2);
//	OLED_SetBrush(&oled,PEN_COLOR_TRANSPARENT);
//	OLED_SetCursor(&oled,24,50);
//	OLED_DrawString(&oled,"Hello world");
//	
//	//Chinese 
//	
//	OLED_SetFont(&oled,&hyjk16);
//	OLED_SetCursor(&oled,50,28);
//	OLED_DrawString(&oled,"朔");

//	
//	
//	OLED_SetFont(&oled,&default_font);
//	OLED_SetCursor(&oled,58,64);
//	OLED_Printf(&oled,"%d/%d/%d",2026,03,04);
//	
//	OLED_SetCursor(&oled,108,14);
//	OLED_Printf(&oled,"Yo");
//	
//	OLED_SendBuffer(&oled);
//	
//	

// draw dot
	OLED_Clear(&oled);
	
	OLED_SetPen(&oled,PEN_COLOR_WHITE,3);
	OLED_SetCursor(&oled,20,30);
	OLED_DrawDot(&oled);
	OLED_SendBuffer(&oled);
	Delay(500);
	


	
	for(uint8_t i =1; i<8; i++)
	{
		OLED_Clear(&oled);
		OLED_MoveCursorX(&oled,10);
		OLED_DrawDot(&oled);
		OLED_SendBuffer(&oled);
		Delay(500);


	}

//	Circle


//	OLED_SetPen(&oled,PEN_COLOR_WHITE,2);
//	OLED_SetBrush(&oled,BRUSH_WHITE);
//	OLED_SetCursor(&oled,65,30);
//	OLED_DrawCircle(&oled,5);

//	OLED_SendBuffer(&oled);



	while(1)
	{

	}
}

void My_SoftwareI2C_Init(void)
{
	si2c.SCL_GPIOx = GPIOB;
	si2c.SCL_GPIO_Pin = GPIO_Pin_8;
	si2c.SDA_GPIOx = GPIOB;
	si2c.SDA_GPIO_Pin = GPIO_Pin_9;
	
	My_SI2C_Init(&si2c);
}

int i2c_write_bytes(uint8_t addr, const uint8_t *pdata, uint16_t size)
{
	return My_SI2C_SendBytes(&si2c,addr,pdata,size);
}

void My_OLEDScreen_Init(void)
{
	OLED_InitTypeDef OLED_InitSturct;
	OLED_InitSturct.i2c_write_cb = i2c_write_bytes;
	OLED_Init(&oled,&OLED_InitSturct);
	
}
