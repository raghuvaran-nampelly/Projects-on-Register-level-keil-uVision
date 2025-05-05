#include "4x4_keypad.h"
#include "km_lcd.h"
#include "FP_commands.h"
#include "Systick_Delay.h"
#include "mapping.h"
#include "string.h"

#define MAX_USERS 10  // maxnumber of users
unsigned short id_mapping_buffer[MAX_USERS * 2];  //buffer store km id templat id
int id_index = 0;
// unsigned short km_id;
int enroll_status;
int template_id;
int enroll_status;
extern int ID;


/*
void Map_ID_Template(void)
	{
		int digit_count = 0;

		char key;
		unsigned short km_id = 0;      // Initialize km_id to 0
    unsigned short template_id = 0; // Initialize template_id to 0

		 if (id_index >= MAX_USERS * 2) 
			 {
					KM_LCD_Write_Cmd(0x01);
					KM_LCD_Write_Str((unsigned char *)"Buffer Full");
					return;
				}

 //enter 4digit ID from keypad
				
    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"enter 4-digit ID:");
    KM_LCD_Write_Cmd(0xC0);

   

    while (digit_count < 4)
			{
        key = keys_scan();
        if(key>='0' && key <= '9') 
					{
            km_id =km_id *10 +(key - '0');//convert char to integer
            KM_LCD_Write_Data(key);
            digit_count++;
			      KM_Delay_ms(200);

					}
			}
    
    //Start enrollment
          KM_Delay_ms(1000);
				EnrollFingerprint();
		//Get latest template ID
    template_id=ID;// assign latest template id
		 

		


    if(template_id <= 0) 
			{
        KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"Error: No Template ID");
        
			}

    //Step 4: Store KM ID & template id in buffer

    id_mapping_buffer[id_index++] = km_id;       //store 4-digit KM ID [Odd index]
    id_mapping_buffer[id_index++] = template_id; //store template ID [Even index]

    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"ID Stored!");
    KM_Delay_ms(1000);
	
   Verify_KM_ID();
		
}
	






void Verify_KM_ID(void) 
	{
		int i;
		 char key;
    int digit_count = 0;
		unsigned short input_km_id = 0;
		 unsigned short template_id = 0;
		
		
    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"ENTER 4-DIGIT KM ID:");
    KM_LCD_Write_Cmd(0xC0);

    
   
    
    // Step 1: enter 4 digit km id 
    while (digit_count < 4)
			{
        key = keys_scan();
        if (key >= '0' && key <= '9') 
					{
            input_km_id = input_km_id * 10 + (key - '0');  // Convert char to integer
            KM_LCD_Write_Data(key);
            digit_count++;
						KM_Delay_ms(200);
					}
			}

    //Step 2:search for km id in the buffer
   
    for (i = 0; i < id_index; i += 2) 
			{
        if (id_mapping_buffer[i] == input_km_id) 
				{
            template_id = id_mapping_buffer[i + 1];  // Get corresponding template ID
            break;
        }
			}

    //step 3:check if KM ID exists
    if (template_id == 0) 
			{
        KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"ID NOT FOUND!");
        return;
			}

    // step 4: Call VerifyFingerprint_LCD with the found template ID
   KM_Delay_ms(1000);

   // VerifyFingerprint_LCD(template_id);
			
			
		
}
*/
char entered_pass[7]; // Store the entered password
char correct_pass[] = "123456"; // fixed password
char person_id[5];
int moisture_flag=0;




void Enter_Pass_Enroll(void)
{
		char key;

		int i=0;
		int j=0;

		KM_LCD_Write_Cmd(0x01); 
	KM_LCD_Write_Str((unsigned char*)"Enter Password:");
		KM_LCD_Write_Cmd(0xC0); 
	while(i < 6)
	{
    key = keys_scan();  // Read keypad input
    if (key != 0)
    {
        entered_pass[i] = key;
				KM_LCD_Write_Data(key); //display on lcd
        KM_Delay_ms(200);  // add some delay
       i++;
    }
		
	}
	entered_pass[6] = '\0';  //null terminate the string


	KM_LCD_Write_Cmd(0xC7);
	KM_LCD_Write_Str((unsigned char*)"ENTER #");



	while (1)  
	{
    key = keys_scan();
    if (key == '#') break;
	}

// Check password
	KM_LCD_Write_Cmd(0x80);
	if (strcmp(entered_pass, correct_pass) == 0)//Compare entered password
	{
    KM_LCD_Write_Str((unsigned char*)"ACCESS GRANTED");
		KM_Delay_ms(1000);
    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char*)"ENTROLLMENT STARTED");
		KM_Delay_ms(2000);
		KM_LCD_Write_Cmd(0x01);
	  KM_LCD_Write_Str((unsigned char*)"READY TO ENROLL");
			KM_Delay_ms(1000);

		KM_LCD_Write_Cmd(0x01); 
		
			Map_ID_Template();//****************************************CALL_ENROLLMENT**********************************
		
	  KM_Delay_ms(2000);
				
	}
	
	else
		{
			KM_LCD_Write_Str((unsigned char*)"WRONG PASSWORD");
			KM_Delay_ms(1000);
			KM_LCD_Write_Cmd(0x01);  // Clear screen
		//	KM_LCD_Write_Cmd(0x01);
			KM_LCD_Write_Str((unsigned char*)"Press Reset");
			KM_Delay_ms(2000);
		}


}
	







/*

void Map_ID_Template(void)
{
	int digit_count = 0;
	char key;
	unsigned short km_id = 0;      // Initialize km_id to 0
  unsigned short template_id = 0; // Initialize template_id to 0

		 if (id_index >= MAX_USERS * 2) 
			 {
					KM_LCD_Write_Cmd(0x01);
					KM_LCD_Write_Str((unsigned char *)"Buffer Full");
					return;
				}

 //enter 4digit ID from keypad
				
    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"enter 4-digit ID:");
    KM_LCD_Write_Cmd(0xC0);

   

    while (digit_count < 4)
			{
        key = keys_scan();
        if(key>='0' && key <= '9') 
					{
            km_id =km_id *10 +(key - '0');//convert char to integer
            KM_LCD_Write_Data(key);
            digit_count++;
			      KM_Delay_ms(200);

					}
					else if (key == '#')  // If '#' is pressed, exit loop
            {
                KM_LCD_Write_Cmd(0x01);
                KM_LCD_Write_Str((unsigned char *)"Enrollment Stopped");
                	Verify_KM_ID();

            }
					
			}
    
    //Start enrollment
          KM_Delay_ms(1000);
				EnrollFingerprint();
		//Get latest template ID
    template_id=ID;// assign latest template id
		 

		


    if(template_id <= 0) 
			{
        KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"Error: No Template ID");
        
			}

    //Step 4: Store KM ID & template id in buffer

    id_mapping_buffer[id_index++] = km_id;       //store 4-digit KM ID [Odd index]
    id_mapping_buffer[id_index++] = template_id; //store template ID [Even index]

    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"ID Stored!");
    KM_Delay_ms(1000);
			
		
	Verify_KM_ID();
   
	
	
}

*/

void Map_ID_Template(void)
{
    char key;

    while (1) // Keep enrolling until '#' is pressed
    {
        int digit_count = 0;
        unsigned short km_id = 0; // Reset KM ID for each iteration

        if (id_index >= MAX_USERS * 2) // Buffer limit check
        {
            KM_LCD_Write_Cmd(0x01);
            KM_LCD_Write_Str((unsigned char *)"Buffer Full");
            KM_Delay_ms(1000);
            return;
        }

        // Step 1: Prompt for 4-digit KM ID
        KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"Enter 4-digit ID:");
        KM_LCD_Write_Cmd(0xC0);

        while (digit_count < 4)  // Get exactly 4 digits
        {
            key = keys_scan();
            if (key >= '0' && key <= '9') 
            {
                km_id = km_id * 10 + (key - '0');  // Convert char to integer
                KM_LCD_Write_Data(key);
                digit_count++;
                KM_Delay_ms(200);
            }
            else if (key == '#')  // If '#' is pressed, exit enrollment mode
            {
                KM_LCD_Write_Cmd(0x01);
                KM_LCD_Write_Str((unsigned char *)"Exiting Enrollment");
                KM_Delay_ms(1000);
                return; // Go back to Admin Mode
            }
        }

        KM_Delay_ms(500);

        // Step 2: Call EnrollFingerprint function with KM ID
        EnrollFingerprint();
       
        // Step 3: Store KM ID in buffer
        id_mapping_buffer[id_index++] = km_id;

        KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"ID Stored!");
        KM_Delay_ms(1000);
    }
}




void Verify_KM_ID(void) 
{
    int i;
    char key;
    int digit_count = 0;
    unsigned short input_km_id = 0;
    int id_found = 0;  // Flag to check if ID exists

    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"ENTER 4-DIGIT KM ID:");
    KM_LCD_Write_Cmd(0xC0);

    // Step 1: Enter 4-digit KM ID 
    while (digit_count < 4)
    {
        key = keys_scan();
        if (key >= '0' && key <= '9') 
        {
            input_km_id = input_km_id * 10 + (key - '0'); //Convert char to integer
            KM_LCD_Write_Data(key);
            digit_count++;
            KM_Delay_ms(200);
        }
    }

    // Step 2: Search for KM ID in the buffer
    for (i = 0; i < id_index; i++)  // Iterate over all stored IDs
    {
        if (id_mapping_buffer[i] == input_km_id) 
        {
            id_found = 1;  // KM ID exists
            break;
        }
    }

    // Step 3: If ID not found, print error
    if (!id_found) 
    {
        KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"ID NOT FOUND!");
        return;
    }

    // Step 4: Perform 1:N verification if ID is found
    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"Place Finger");
    KM_Delay_ms(1000);
    
    IdentifyFingerprint_LCD();  // Call 1:N verification function
}


