
#include "../stm32f401rbt6.h"
#include "UART.h"
unsigned char timeout_flag=0;

void Uart1_init_config(void)
{
	RCC_AHB1ENR |= 0X1<<0; //to enable PORT A clock
	RCC_APB2ENR |= (0x1<<4); //to enable UART1 clock
	GPIOA_MODER &= 0xFFC3FFFF;
	GPIOA_MODER |= 0x00280000; //load "10" into bit fields (21_20),(19_18) bit fields to enable PA10,PA9 pins as alternate output and works as UART1 Rx,Tx pins
	GPIOA_AFRH  &= 0xFFFFF00F;
	GPIOA_AFRH  |= 0x00000770; //To Select UART1_Tx and UART1_RX functionality using Mux register (Load ‘0111’ in GPIO_PORTA AFRH9,AFRH10 )
	
	UART1_BRR =0x683; //Set 9600 baud rate in BRR (0x683)
	UART1_CR1 |=(0x1<<13); //Set 13th bit in CR1 register to enable USART1
	UART1_CR1 |=(0x1<<2); //Set 2nd bit in CR1 register to enable RE
	UART1_CR1 |=(0x1<<3); //Set 3rd bit in CR1 register to enable TE
	//UART1_CR1 |= 0x1<<5; // to enable RXNE Interrupt
}

 


void Uart1_OutChar(unsigned char data)
{
while(!(UART1_SR & (0X01<<7)))	//WAIT UNTIL TXE (TRANSIMT DATA REGISTER EMPTY) FLAG IS SET
	{
		;
	}
	
			 UART1_DR=data;	//WRITE THE DATA TO DATA REGISTER DR TO SEND IT
			
}







void Uart1_Outstr(unsigned char str[])
{
	int i=0;
	while(i<12)
	{
		
		Uart1_OutChar(str[i]);
		i++;
	}

}






int Uart1_InChar_TIMEOUT (unsigned int timeout)
{
	while(timeout>0)
	{
			if(	UART1_SR & (0X01<<5))// Check if RXNE (Receive Data Register Not Empty) flag is set
			{
				return (UART1_DR );// Return received character
			}
			timeout--;// Decrement timeout
	}
		timeout_flag=1;

	return -1;// Return -1 if timeout occurs


}

