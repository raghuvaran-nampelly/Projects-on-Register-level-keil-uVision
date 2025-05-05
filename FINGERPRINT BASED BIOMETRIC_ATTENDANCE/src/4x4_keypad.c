#include "stm32f401rbt6.h"
#include "systick_delay.h"
#include "4x4_keypad.h"
#include <string.h>
#include "km_lcd.h"
#define R1_pin 0x01<<4
#define C1_pin 0x01<<2




void keypad_init_config(void)
	{
		RCC_AHB1ENR |=0X01<<0|0X01<<2; //enable clock for port a and port b
		GPIOA_MODER &= ~(0xFF<<8);    
		GPIOA_MODER |= (0x55<<8);     
		
		GPIOC_MODER &= ~(0xFF<<4);  //set input mode  
		GPIOC_PUPDR &= ~(0xFF<<4);   //clear pupdr bits
		GPIOC_PUPDR |= (0xAA<<4);     //make pull down
	}







unsigned char keys_scan(void)
{
	int row,col;
	char key[4][4]={{'1','2','3','A'},
									{'4','5','6','B'},
									{'7','8','9','C'},
									{'*','0','#','D'}};
	
	for(row=0;row<4;row++)
		{
			//GPIOA_ODR |=R1_pin<<row;   //row start from pA4
			
			for(col=0;col<4;col++)
			{
				GPIOA_ODR |=R1_pin<<row;  

				if(GPIOC_IDR & C1_pin<<col)	//col start from pC2
				{
						GPIOA_ODR &=~(R1_pin<<row);
					return key[row][col];
				}
					GPIOA_ODR &=~(R1_pin<<row);

			}
			GPIOA_ODR &=~(R1_pin<<row);

		}
		return 0;
	}

