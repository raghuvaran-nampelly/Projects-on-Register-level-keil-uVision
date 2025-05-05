#include "4x4_keypad.h"
#include "stm32f401rbt6.h"
#include "FP_commands.h"
#include "km_lcd.h"
#include "Systick_Delay.h"
#include "string.h"

////char entered_pass[7]; // Store the entered password
////char correct_pass[] = "123456"; // fixed password
////char person_id[5];
////int moisture_flag=0;
////	char key;




//void Enter_UserID(void)
//{
//		int i=0;
//		int j=0;

//		KM_LCD_Write_Cmd(0x01); 
//	KM_LCD_Write_Str((unsigned char*)"Enter Password:");
//		KM_LCD_Write_Cmd(0xC0); 
//	while(i < 6)
//	{
//    key = keys_scan();  // Read keypad input
//    if (key != 0)
//    {
//        entered_pass[i] = key;
//				KM_LCD_Write_Data(key); //display on lcd
//        KM_Delay_ms(200);  // add some delay
//       i++;
//    }
//		
//	}
//	entered_pass[6] = '\0';  //null terminate the string


//	KM_LCD_Write_Cmd(0xC7);
//	KM_LCD_Write_Str((unsigned char*)"ENTER #");



//	while (1)  
//	{
//    key = keys_scan();
//    if (key == '#') break;
//	}

//// Check password
//	KM_LCD_Write_Cmd(0x80);
//	if (strcmp(entered_pass, correct_pass) == 0)//Compare entered password
//	{
//    KM_LCD_Write_Str((unsigned char*)"ACCESS GRANTED");
//		KM_Delay_ms(1000);
//    KM_LCD_Write_Cmd(0x01);
//    KM_LCD_Write_Str((unsigned char*)"ENTROLLMENT STARTED");
//		KM_Delay_ms(2000);
//		KM_LCD_Write_Cmd(0x01);
//	  KM_LCD_Write_Str((unsigned char*)"READY TO ENROLL");
//		
//		
//		
//		KM_LCD_Write_Cmd(0x01); 
//		KM_LCD_Write_Str((unsigned char*)"Enter UserID 1:");
//		KM_LCD_Write_Cmd(0xC0); 
//	while(j < 4)
//	{
//    key = keys_scan();  // Read keypad input
//    if (key != 0)
//    {
//        person_id[i] = key;
//				KM_LCD_Write_Data(key); //display on lcd
//        KM_Delay_ms(200);  // add some delay
//       j++;
//    }
//		
//	}
//	person_id[4] = '\0';  //null terminate the string


//	KM_LCD_Write_Cmd(0xC7);
//	KM_LCD_Write_Str((unsigned char*)"ENTER #");



//	while (1)  
//	{
//    key = keys_scan();
//    if(key == '#') 
//			break;
//	}
//		EnrollFingerprint();//****************************************CALL_ENROLLMENT**********************************
//		
//	  KM_Delay_ms(2000);
//				while (1)  
//							{
//									key = keys_scan();
//									if (key == '3') //exit mode
//									{
//										KM_LCD_Write_Cmd(0x01);
//										KM_LCD_Write_Str((unsigned char*)"exiting...");
//										KM_Delay_ms(1000);

//										KM_LCD_Write_Cmd(0x01);
//										KM_LCD_Write_Str((unsigned char*)"reset");
//										KM_Delay_ms(2000);
//										return;  // Return to Config Menu
//									}
//							}
//   }

//	
//	else
//		{
//			KM_LCD_Write_Str((unsigned char*)"WRONG PASSWORD");
//			KM_Delay_ms(1000);
//			KM_LCD_Write_Cmd(0x01);  // Clear screen
//		//	KM_LCD_Write_Cmd(0x01);
//			KM_LCD_Write_Str((unsigned char*)"Press Reset");
//			KM_Delay_ms(2000);
//		}


//}

