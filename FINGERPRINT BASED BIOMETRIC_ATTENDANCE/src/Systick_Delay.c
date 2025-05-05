/**********************************************************
*   FUN_NAME( ) :void KM_mdelay(int n)
************************************************************
*Description     :3) Write a program to implement generic delay functions using Sys Tick timer with polling method.
                void KM_mdelay(int );  
                void KM_udelay(int );  
*Arguments       :n
*Return Type     :no return type
*Notes           :this delay function will generate exact vaalue of delay in micro and milli
*************************************************************/




#include "../stm32f401rbt6.h"

#define MAX_32       2147483647
 int Task1_flag=0;
 int Task2_flag=0;
 int Tick1=0,Tick2=0;
 int Tick=0;



void SysTick_Handler() 
{
		if(Tick==MAX_32)        
		{
			Tick=0;
		}
		
	
		if(Tick1==999)
		{
			Task1_flag=1;
			Tick1=0;
		}
		if(Tick2==4999)
		{
			Task2_flag=1;
			Tick2=0;
		}
		Tick++;  
	  Tick1++;
	  Tick2++;
			
}

void KM_Delay_ms(unsigned int delay)
{
			unsigned int delay_until;
			if(Tick+delay>=MAX_32)		//make Tick1 COUNT TO ZER AFTER MAX COUNT
				{
					Tick=0;							//CLEAR TICK1
				}
				delay_until=Tick+delay;
				while(Tick<delay_until);

}



void Systick_init(void)
{
	STK_LOAD = 16000;   
	STK_VAL = 123; 
	STK_CTRL |= 0X04;   
	STK_CTRL |= 0X01;   
	STK_CTRL |= 0X01<<1; 

}




