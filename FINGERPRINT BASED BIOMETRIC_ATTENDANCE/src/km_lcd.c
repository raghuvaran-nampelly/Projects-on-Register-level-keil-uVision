/*
*Description :This file contain all the lcd functions
*/
#include "../stm32f401rbt6.h"
#include "km_lcd.h"
//#include "gpio.h"
#include "Systick_Delay.h"

	
void Lcd_gpiob_init_config(void)
{
	RCC_AHB1ENR |=0X01<<1;
	GPIOB_MODER &=0xfffcf000;
	GPIOB_MODER |=0x00010555;
}

	

void KM_LCD_Init(void)
{
	  KM_LCD_Write_Cmd(0x33);	//used to initialize the display in 4-bit mode
		KM_Delay_ms(30);
		KM_LCD_Write_Cmd(0x32);	// used to set the LCD to 4-bit mode
		KM_LCD_Write_Cmd(0x0C);	//Display on, cursor off
		KM_LCD_Write_Cmd(0x01);	//Display on, cursor off
		
}



void KM_LCD_Write_Str(unsigned char str[])
{
	int i=0;
	while(str[i]!='\0')
	{
		KM_LCD_Write_Data(str[i]);//SEND CHARACTER BY CHARACTER
		i++;
	}
}


void KM_LCD_Write_Cmd( unsigned char data)
{
	 GPIOB_ODR &=~(0x01<<4);	//CLEAR 4TH BIT  TO ENABLE COMMAND MODE
	 write_high_nibble(data);	//CALL HIGHER NIBBLE COMMAND
	 write_low_nibble(data);	//CALL LOWER NIBBLE COMMAND
	 

}


void KM_LCD_Write_Data( unsigned char data)
{
	GPIOB_ODR |=(0X01<<4);//SET 4TH BIT TO 1 TO ENABLE DATAMODE
	write_high_nibble(data);//CALL HIGHER NIBBLE DATA
	write_low_nibble(data);//CALL LOWER NIBBLE DATA
}


void write_high_nibble( unsigned char data )
{
	data=data>>4;	//SHIFT HIGHER NIBBLE DATA TO LOWER NIBBLE
	GPIOB_ODR &=~(0x0f);	//CLEAR HIGHER NIBBLE DATA IN GPIO_ODR
	GPIOB_ODR |= data;	//TRANSFER THE DATA TO GPIOB_ODR
	GPIOB_ODR |=0x01<<8;	//SET 8TH BIT IN ODR TO ENABLE
	KM_Delay_ms(10);	//CALL SOME KM_Delay
	GPIOB_ODR &=~(0x01<<8);	//NOW DISABLE EN 
	
}






void write_low_nibble(unsigned char data)
{
	data &=~(0xf0);	//EXTRACT LOWER NIBBLE
	GPIOB_ODR &=~(0x0f);	//CLEAR LOWER NIBBLE IN ODR
	GPIOB_ODR |=data;	//TRANSFER DATA TO ODR
	GPIOB_ODR |=0x01<<8;	//SET ENABLE
	KM_Delay_ms(10);		//CALL SOME KM_Delay
	GPIOB_ODR &=~(0x01<<8);	//CLEAR ENABLE
}

