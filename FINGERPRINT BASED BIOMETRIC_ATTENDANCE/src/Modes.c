
#include "km_lcd.h"
#include "4x4_keypad.h"
#include "FP_commands.h"
#include "Systick_Delay.h"
#include "poss_id.h"
#include <stdio.h>
#include "mapping.h"
int res;


void Admin_mode(void)
{
    char buffer[20];
    char key;

    while (1) // Keep running the menu until the user exits
    {
        KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"1.Enroll 2.Delete");
        KM_LCD_Write_Cmd(0xC0);
        KM_LCD_Write_Str((unsigned char *)"3.Count 4.EXIT");

        while (!(key = keys_scan())); // Wait for a key press

        switch (key)
        {
			case '0':
					Verify_KM_ID();
								break;
					
            case '1':
                Map_ID_Template();  // Enter Enrollment Mode
                break;
                
            case '2':
                DeleteAll();
                KM_LCD_Write_Cmd(0x01); // Clear screen
                KM_LCD_Write_Str((unsigned char *)"DELETING DATA...");
                KM_Delay_ms(1000);
                break;

            case '3':
                res = GetEnrollCount();
                KM_LCD_Write_Cmd(0x01);
                sprintf(buffer, "Total count:%04X", res);
                KM_LCD_Write_Str((unsigned char *)buffer);
                KM_Delay_ms(1000);
                break;

            case '4':
                KM_LCD_Write_Cmd(0x01); // Clear screen
                KM_LCD_Write_Str((unsigned char *)"System Exiting...");
                KM_Delay_ms(1000);
                return;  // Exit the function, returning to the main loop
        }
    }
}




void User_mode(void)
{
	char key;
		KM_LCD_Write_Str((unsigned char *)"Entered into Usermode");
		KM_LCD_Write_Cmd(0xC0);
	KM_LCD_Write_Str((unsigned char *)"1.verify 2.identif");
	
		while(!(key=keys_scan()));
		
		switch(key)
		{
			case '1':Verify_KM_ID();
								break;
			case '2':IdentifyFingerprint_LCD();
								break;

		}
}