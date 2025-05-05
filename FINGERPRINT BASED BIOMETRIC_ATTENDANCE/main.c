#include "stm32f401rbt6.h"
#include "Systick_Delay.h"
#include "UART.h"
#include "FP_commands.h"
#include "km_lcd.h"
#include "4x4_keypad.h"
#include "poss_id.h"
#include "Modes.h"
#include "mapping.h"
#include "i2c.h"

#define RTC_ADDR 0x68
unsigned char today_date[30]={0x00,0x08,0x10,0x03,0x28,0x03,0x25};


extern int Task1_flag,Task2_flag;
char receive[30];
unsigned char open[]     		 = {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x01};


unsigned int sw_ent = 0;
unsigned int sw_up = 0;
unsigned int sw_dn = 0;

//void EXTI9_5_IRQHandler(void)
//{
//	if((EXTI_PR)&(0x1<<8))
//	{
//		sw_up=1;
//		EXTI_PR |= (0x1<<8); // EXTI_PR 8th bit is set, so that interrupt will be cleared
//	}
//  
//	if((EXTI_PR)&(0x1<<9))
//	{
//		sw_dn=1;
//		EXTI_PR |= (0x1<<9); // EXTI_PR 9th bit is set, so that interrupt will be cleared
//	}
//}



void EXTI15_10_IRQHandler(void)
{
	if((EXTI_PR)&(0x1<<10))
	{
		
		sw_ent=1;
		EXTI_PR |= (0x1<<10); // EXTI_PR is set, so that interrupt will�be�cleared
	}
}








void Interpt_sw_initConfig(void)
{
			RCC_APB2ENR |= 0x1<<14; //set 14th bit in RCC_APB2ENR to enable SYSCFG 

			GPIOC_PUPDR &= 0xFFC0FFFF; //clear 21,20,19,18,17,16 bits in GPIOC_PUPDR
			GPIOC_PUPDR |= 0X00150000; //load "01" into bit fields (21,20)(19,18),(17,16) in GPIOC_PUPDR to configure as pull_up

			SYSCFG_EXTICR3 &= (0XFFFFF000);//clear 7_0 bit fields in SYSCFG_EXTRIC3
			SYSCFG_EXTICR3 |= (0x00000222); // load "0010" into 11_8, 7_4 and 3_0 bit fields in SYSCFG_EXTRIC3

			EXTI_FTSR |= (0x1<<8); // set 8th bit in EXTI_FTSR
			EXTI_FTSR |= (0x1<<9); // set 9th bit in EXTI_FTSR
			EXTI_FTSR |= (0x1<<10); // set 10th bit in EXTI_FTSR

			EXTI_IMR |= (0x1<<10); // set 10th bit in EXTI_IMR 
			EXTI_IMR |= (0x1<<8); // set 8th bit in EXTI_IMR 
			EXTI_IMR |= (0x1<<9); // set 9th bit in EXTI_IMR 
	
			NVIC_ISER0 |= (0X1<<23); //set 23rd bit in NVIC_ISER0
			NVIC_ISER1 |= (0X1<<8); //set 8th bit in NVIC_ISER1
}
	
 

 
 
 

void ICPCK_Config(void)
{
	RCC_AHB1ENR |= (0x01 << 0);	//Set 0th bit of RCC_AHB1ENR to Enable Port A clock

	RCC_APB2ENR |= (0x01 << 14);         //set 14th bit of RCC_APB2ENR to Enable SYSCFG clock
		
	GPIOA_MODER &= ~(0x03 << (8*2));     //clear 13-12 bits of GPIOA_MODER (by default it is in input mode only)
	GPIOA_PUPDR &= ~(0x03 << (8*2));     //clear 13-12 bits of GPIOA_PUPDR
	GPIOA_PUPDR |=  (0x02 << (8*2));     //load "10" into 13-12 bit fields in GPIOA_PUPDR to make PC0 as pull-down

	SYSCFG_EXTICR3 &= 0xFFF0;
	EXTI_RTSR |= (0x01 << 8);
	EXTI_FTSR |= (0x01 << 8);           //Enable Falling Edge Trigger
	EXTI_IMR  |= (0x01 << 8);
  NVIC_ISER0 |= (0x01 << 23);         //set 23rd bit in NVIC_ISTR0 to enable NVIC 23rd interrupt?line

}



 


int main()
{
	char key;
    Systick_init();
		Lcd_gpiob_init_config();
		KM_LCD_Init();
		Uart1_init_config();
		KM_I2C1_Init();
		KM_I2C1_Config();
		ICPCK_Config();
		keypad_init_config();
		KM_LCD_Write_Str((unsigned char *)"BIOMETRIC ATTEND");
		KM_LCD_Write_Cmd(0xC0);
		KM_LCD_Write_Str((unsigned char *)"ANCE SYSTEM");
		KM_Delay_ms(1000);
		KM_LCD_Write_Cmd(0x01);


	//	Interpt_sw_initConfig();
		
		
		
		KM_LCD_Write_Str((unsigned char *)"Select Mode");
		KM_LCD_Write_Cmd(0xC0);
		KM_LCD_Write_Str((unsigned char *)"1.User 2.Admin");
			KM_I2C_Mem_Write(RTC_ADDR,0x00,0x1,today_date,7);

		while(1)
		{
			
		if(Task1_flag==1)
							{
								
								Task1_ReadRTC_WriteLCD_1Sec();
								Task1_flag=0;
							}
		}
		while(!(key=keys_scan()));
		switch(key)
			{
				case '1':Admin_mode();
								break;
				case '2':User_mode();
								break;
			}

}

