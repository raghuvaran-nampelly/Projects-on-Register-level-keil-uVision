#include "FP_commands.h"
#include "UART.h"
#include "Systick_Delay.h"
#include "km_lcd.h"
#include "stm32f401rbt6.h"
#include <stdio.h>
#include "4x4_keypad.h"
#include "i2c.h"
//#include "WIFI.h"

#define RTC_ADDR 0x68

	int status=0;
int result=0;
	char FP_Buffer[12];
	char buffer[15];
	extern int timeout_flag;
	unsigned short error_code;
	char User_ID[5];
	unsigned char packet[12];  
 int ID=0;


int led_flag=0;
  
//#define TIMEOUT_VALUE     10000 




// unsigned char open[]           	  = {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x01};
// unsigned char close[]              = {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x02,0x01};
// unsigned char led_on[]             = {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x12,0x00,0x13,0x01};
// unsigned char led_off[]            = {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x12,0x01};
// unsigned char enroll_start[]       = {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x22,0x00,0x23,0x01};
// unsigned char is_press_finger[]	 	= {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x26,0x00,0x26,0x01};
// unsigned char capture_finger[]			= {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x01};
// unsigned char enroll1[]       			= {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x23,0x00,0x23,0x01};
// unsigned char enroll2[]       			= {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x24,0x00,0x24,0x01};
// unsigned char enroll3[]       			= {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x25,0x00,0x25,0x01};
// unsigned char check_enroll[]				= {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x21,0x00,0x22,0x01};
// unsigned char get_enroll[]					= {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x20,0x01};
// unsigned char identify[]    	 		  = {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x51,0x00,0x51,0x01};
// unsigned char verify_finger[]      = {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x50,0x00,0x51,0x01};
// unsigned char delete_finger[]      = {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x40,0x00,0x41,0x01};
// unsigned char delete_all_finger[]  = {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x41,0x00,0x41,0x01};


 
 
 
 void EXTI9_5_IRQHandler(void) 
	{
    if (EXTI_PR & (0x01 << 8))  // Check if PA6 triggered interrupt
    { 
			if(GPIOA_IDR & 0x01<<8)
			{
				Send_Command_Check_Response(CMOSLED,1, 100000); // Turn LED on when finger placed
				led_flag=1;
			}

			else
			{
				 Send_Command_Check_Response(CMOSLED,0, 100000); // Turn LED off when finger removed
					led_flag=0;
			}
        
						
			
        
        EXTI_PR |= (0x01 << 8);  // Clear the pending interrupt flag
    }
	}
	
	
	
	
	
	
	
	
	
 void Send_Command_Check_Response(unsigned char command, unsigned short parameter,unsigned int Timeout_Value)
	{
		PreparePacket(packet, command, parameter); 
		Uart1_Outstr(packet);
		Fingerprint_Response(Timeout_Value);        //Read 12-byte response into FP_Buffer[]
	}

	
	
void Fingerprint_Response(unsigned int Timeout_Value)
	{
		int i;
		int received_data; 
			
		for(i=0;i<12;i++)
		{
		received_data = Uart1_InChar_TIMEOUT(Timeout_Value);
		if(received_data == -1)
		{
			return;			
		}
		
		FP_Buffer[i] = (unsigned char)received_data;
		}
	}

		
		
		


	void PreparePacket(unsigned char packet[],unsigned char command,unsigned char parameter)
{	
	int i;
	unsigned short int checksum=0;

	//Start Header (Fixed)
  packet[0] = 0x55;  
  packet[1] = 0xAA;  

  //Device ID (Default:0x0001)
  packet[2] = 0x01;  
  packet[3] = 0x00;  

  //Parameters
	packet[4] = (parameter & 0xFF);           //lower byte of parameter 
  packet[5] = ((parameter >> 8) & 0xFF);    //higher byte of parameter
  packet[6] = 0x00;  
  packet[7] = 0x00;  
  	
  //Command Byte
  packet[8] = command;  
  packet[9] = 0x00;  
 
  //Checksum (Sum of bytes except header)
	for (i = 0; i <= 9; i++)
  {
		checksum += packet[i];
  }
  packet[10] = (checksum & 0xFF);              //lower byte of checksum 
  packet[11] = ((checksum >> 8) & 0xFF);       //higher byte of checksum
	
} 
 
 
	
		
		

  



	
	
	

int GetErrorCode(void)
{
	int error = (FP_Buffer[5] << 8) | FP_Buffer[4];  //Extract the 2-byte error code from positions 4 and 5
	return error;
}




int IsFingerPlaced(void)
{
Send_Command_Check_Response(ISPRESSFINGER,0, 250000); // Enroll start
  error_code = GetErrorCode();               //Get the error code

  //Handle errors first
  switch (error_code)
  {
		case NACK_COMM_ERR:
			return NACK_COMM_ERR;         //Communication error

    case NACK_DEV_ERR:
			return NACK_DEV_ERR;          //Device error

    case NACK_FINGER_IS_NOT_PRESSED:
			return NACK_FINGER_IS_NOT_PRESSED;   //No finger pressed

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;                 //Timeout error

    default:
			return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;    //If no error, check for finger presence
  }
}





int CaptureFinger(void)
{
Send_Command_Check_Response(CAPTUREFINGER,0, 500000); // Enroll start

  error_code = GetErrorCode();            //Get the error code

  //Handle specific error cases
  switch (error_code)
  {
		case NACK_BAD_FINGER:
			return NACK_BAD_FINGER;             //Poor fingerprint quality
		
		case NACK_FINGER_IS_NOT_PRESSED:
			return NACK_FINGER_IS_NOT_PRESSED;  //No finger detected

    case NACK_COMM_ERR:
      return NACK_COMM_ERR;              //Communication error

    case NACK_DEV_ERR:
      return NACK_DEV_ERR;               //Device error

    case NACK_TIMEOUT:
      return NACK_TIMEOUT;              //Timeout error

    default:
			 //If no error, check for success response
       return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;  
    }
}




int EnrollStart(unsigned short int ID)
{
	
	//enroll_start[5] = (unsigned char)ID;
	
	 Send_Command_Check_Response(ENROLLSTART,ID,100000);
	
  error_code = GetErrorCode();                 //Get the error code
	
	//Handle specific errors
  switch (error_code)
  {
		case NACK_IS_ALREADY_USED:
			return NACK_IS_ALREADY_USED;     //ID is already in use

    case NACK_INVALID_POS:
			return NACK_INVALID_POS;        //Invalid ID position (out of range)

    case NACK_DB_IS_FULL:
			return NACK_DB_IS_FULL;        //Database is full

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//If no error, check for success response
      return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;
  }
}







int Enroll1(void)
{
	Send_Command_Check_Response(ENROLL1,0, 700000);

  //Get the error code
	error_code = GetErrorCode();
	
  //Handle specific error cases
  switch (error_code)
  {
		case NACK_BAD_FINGER:
			return NACK_BAD_FINGER;         //Poor fingerprint quality

    case NACK_ENROLL_FAILED:
			return NACK_ENROLL_FAILED;      //Enrollment processing error

    case NACK_TURN_ERR:
			return NACK_TURN_ERR;           //Incorrect enrollment sequence

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//If no error, check for success response
			return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;  
  }
}








int Enroll2(void)
{
		Send_Command_Check_Response(ENROLL2,0, 700000);
  //Get the error code
	error_code = GetErrorCode();
	
  //Handle specific error cases
  switch (error_code)
  {
		case NACK_BAD_FINGER:
			return NACK_BAD_FINGER;         //Poor fingerprint quality

    case NACK_ENROLL_FAILED:
			return NACK_ENROLL_FAILED;      //Enrollment processing error

    case NACK_TURN_ERR:
			return NACK_TURN_ERR;           //Incorrect enrollment sequence

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//If no error, check for success response
			return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;  
  }
}






int Enroll3(void)
{
	Send_Command_Check_Response(ENROLL3,0, 700000);

  //Get the error code
	error_code = GetErrorCode();
	
  //Handle specific error cases
  switch(error_code)
  {
		case NACK_BAD_FINGER:
			return NACK_BAD_FINGER;         //Poor fingerprint quality

    case NACK_ENROLL_FAILED:
			return NACK_ENROLL_FAILED;      //Enrollment processing error

    case NACK_TURN_ERR:
			return NACK_TURN_ERR;           //Incorrect enrollment sequence

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//If no error, check for success response
			return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;  
  }
}







int GetEnrollCount(void)
{
	Send_Command_Check_Response(GETENROLLCOUNT,0, 700000);

  error_code = GetErrorCode();                //Get the error code

  //Check for specific error messages
  switch (error_code)
  {
		case NACK_TIMEOUT:
			return NACK_TIMEOUT;                    //Timeout occurred

    case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;                //No fingerprints are enrolled

    case NACK_COMM_ERR:
			return NACK_COMM_ERR;                   //Communication error

    default:
			return ((FP_Buffer[5] << 8) | FP_Buffer[4]);    //If no error, extract enroll count
		
  }
}








void EnrollFingerprint(void)
{
	
	int step=0;
  int enroll_status =0;
  int enroll_start_status=0;

//	int i;
	
	//entre user ID**********************************************
		
	
	
 ID = GetEnrollCount();  //Get the next available ID
	
  if(ID < 0)  
  {
		switch(ID)
    {
			case NACK_TIMEOUT:	
				KM_LCD_Write_Cmd(0x01);
				KM_LCD_Write_Str((unsigned char *)"ERROR: id fetch timeout");
        break;
			
      case NACK_DB_IS_EMPTY:
				KM_LCD_Write_Cmd(0x01);
				KM_LCD_Write_Str((unsigned char *)"ERROR: DB is empty");
        break;
			
      case NACK_COMM_ERR:
				KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"ERROR: communication failure");
        break;
			
      default:
				KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"ERROR: unknown id error");
    }
    return;
  }
	
	KM_Delay_ms(3000);

	
  //Start Enrollment Process
	 enroll_start_status = EnrollStart(ID);
	
  if(enroll_start_status != SUCCESS)
  {
		switch(enroll_start_status)
    {
			case NACK_IS_ALREADY_USED:	KM_LCD_Write_Cmd(0x01);
																	KM_LCD_Write_Str((unsigned char *)"id used alredy");
																	break;
		
			case NACK_COMM_ERR:					KM_LCD_Write_Cmd(0x01);
																	KM_LCD_Write_Str((unsigned char *)"ERROR: communication error");
																	break;
			
      case NACK_DEV_ERR:					KM_LCD_Write_Cmd(0x01);
																	KM_LCD_Write_Str((unsigned char *)"ERROR: device error");
																	break;
			
      case NACK_TIMEOUT:					KM_LCD_Write_Cmd(0x01);
																	KM_LCD_Write_Str((unsigned char *)"ERROR: Timeout in enrollstart");
																	break;
			
																	default:
																	KM_LCD_Write_Cmd(0x01);
																	KM_LCD_Write_Str((unsigned char *)"ERROR: Enrollstart failed");
    }
    return;
  }
	KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Str((unsigned char *)"ENROLLMENT STARTED");
	
	
	//Capture and enroll fingerprints sequentially
  for(step=1;step <= 3;step++)
  {
		sprintf(buffer,"PLACE FINGER %d",(step));
		KM_LCD_Write_Cmd(0x01);
		KM_LCD_Write_Str((unsigned char *)buffer);
		
		if(step!=1)
		{
			KM_LCD_Write_Cmd(0xC0);
			KM_LCD_Write_Str((unsigned char *)"place same as before");
		}

    //Wait until both touch_flag == 1 (button pressed) and IsFingerPlaced() is successful
    while(!(led_flag == 1 && IsFingerPlaced() == SUCCESS))
    {
      ;
    }

    if(CaptureFinger() != SUCCESS)
    {
			KM_LCD_Write_Cmd(0x01);
			KM_LCD_Write_Str((unsigned char *)" ERROR: finger capture failed");
      return;
    }

        
    switch(step)
    {
			case 1:
				enroll_status = Enroll1();
			  KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"enroll1 success");
			  KM_Delay_ms(1000);
			  break;
			
      case 2:
        enroll_status = Enroll2();
			  KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"enroll2 success");
			  KM_Delay_ms(1000);
        break;
			
      case 3:
        enroll_status = Enroll3();
			  KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"enroll3 success");
			  KM_Delay_ms(1000);
        break;
    }

    if(enroll_status != SUCCESS)
    {
			switch (enroll_status)
      {
				case NACK_BAD_FINGER:
					KM_LCD_Write_Cmd(0x01);
					KM_LCD_Write_Str((unsigned char *)"bad finger");
          break;
				
        case NACK_ENROLL_FAILED:
					KM_LCD_Write_Cmd(0x01);
          KM_LCD_Write_Str((unsigned char *)"enrollment failed");
          break;
				
        case NACK_TURN_ERR:
					KM_LCD_Write_Cmd(0x01);
          KM_LCD_Write_Str((unsigned char *)"wrong sequence");
          break;
				
        case NACK_TIMEOUT:
					KM_LCD_Write_Cmd(0x01);
          KM_LCD_Write_Str((unsigned char *)"Timeout enrollment");
          break;
				
        default:
					KM_LCD_Write_Cmd(0x01);
          KM_LCD_Write_Str((unsigned char *)"Unknowm enrollment");
      }
      return;
    }

		KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"Remove finger");
		
    //Wait for button release (touch_flag == 0) (which also results in finger removal)
    while(led_flag == 1);
  }

	KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Str((unsigned char *)"Enrollment sucessfull");
	KM_Delay_ms(1000);
}







/*********************************************************IDENTIFY[1:N]*****************************************************************************/

void IdentifyFingerprint_LCD(void)
{
	int result=0;
	char buffer[20];

	KM_LCD_Write_Cmd(0x01);
	KM_LCD_Write_Str((unsigned char *)"[1:N] identification");
	
  while(!(led_flag == 1 && IsFingerPlaced() == SUCCESS))
		{
			;
		}

  if(CaptureFinger() != SUCCESS)
  {
		KM_LCD_Write_Cmd(0x01);
	  KM_LCD_Write_Str((unsigned char *)" ERROR: finger capture failed");
    return;
  }	
	
	//KM_Delay_ms(1000);
	
  result = IdentifyFingerprint();  //Call IdentifyFingerprint()

  KM_LCD_Write_Cmd(0x01);         //Clear LCD before displaying new message

  switch (result)
  {
		case NACK_DB_IS_EMPTY:
			KM_LCD_Write_Str((unsigned char *)"ERROR: db is empty");
      break;

    case NACK_IDENTIFY_FAILED:
			KM_LCD_Write_Str((unsigned char *)"ERROR: no match");
      break;

    case NACK_COMM_ERR:
			KM_LCD_Write_Str((unsigned char *)"ERROR: commun failure");
      break;

    case NACK_DEV_ERR:
			KM_LCD_Write_Cmd(0x01);
			KM_LCD_Write_Str((unsigned char *)"ERROR: device failure");
      break;

    case NACK_TIMEOUT:
			KM_LCD_Write_Str((unsigned char *)"ERROR: Timeout");
      break;

    default:
			if(result >= 0)     //If a valid ID is received
      {
				sprintf(buffer, "Id Matched: %04X", result);
        KM_LCD_Write_Str((unsigned char *)buffer);
							KM_LCD_Write_Cmd(0xC0);
				      KM_LCD_Write_Str((unsigned char *)"Attendance Marked");

      }
			
      else
      {
				KM_LCD_Write_Str((unsigned char *)"Identification failed");
      }
      break;
    }

    KM_Delay_ms(2000);  //Delay 
}







int IdentifyFingerprint(void)
{
	Send_Command_Check_Response(IDENTIFY,0, 700000);
	
	error_code = GetErrorCode();        //Get the error code

  switch(error_code)
  {
		case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;       //Database is Empty
		
		case NACK_IDENTIFY_FAILED:
			return NACK_IDENTIFY_FAILED;    //No match found

    case NACK_COMM_ERR:
			return NACK_COMM_ERR;           //Communication error

    case NACK_DEV_ERR:
			return NACK_DEV_ERR;           //Device error

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//Check for success response
      return (FP_Buffer[8] == 0x30) ? ((FP_Buffer[5] << 8) | FP_Buffer[4]) : ERROR_ENROLLMENT_FAIL;  
  }
}






/********************************************VERIFICATION[1:1]*********************************************************************/


void VerifyFingerprint_LCD(int ID)
{
	
  char buffer[20];
	
	KM_LCD_Write_Cmd(0x01);
	KM_LCD_Write_Str((unsigned char *)"[1:1] verification");
	
  while(!(led_flag == 1 && IsFingerPlaced() == SUCCESS))
		{
			;
		}

  if(CaptureFinger() != SUCCESS)
  {
		KM_LCD_Write_Cmd(0x01);
	  KM_LCD_Write_Str((unsigned char *)" ERROR: failed finger capture");
    return;
  }	
	
	result = VerifyFingerprint(ID);  //Call VerifyFingerprint() with ID

  KM_LCD_Write_Cmd(0x01);         //Clear LCD before displaying message

  switch (result)
  {
		case NACK_DB_IS_EMPTY:
			KM_LCD_Write_Str((unsigned char *)"ERROR: db is empty");
      break;

    case NACK_VERIFY_FAILED:
      KM_LCD_Write_Str((unsigned char *)"ERROR: no match");
      break;

    case NACK_COMM_ERR:
      KM_LCD_Write_Str((unsigned char *)"ERROR: comm failure");
      break;

    case NACK_DEV_ERR:
      KM_LCD_Write_Str((unsigned char *)"ERROR: device error");
      break;

    case NACK_TIMEOUT:
      KM_LCD_Write_Str((unsigned char *)"ERROR: timeout");
      break;

    case SUCCESS:
		  sprintf(buffer, "ID MATCHED:OK");
		  KM_LCD_Write_Cmd(0x80);
     
      KM_LCD_Write_Cmd(0xC0);		
      KM_LCD_Write_Str((unsigned char *)"Attendance Marked");
      break;

    default:
      KM_LCD_Write_Str((unsigned char *)"ERROR: verify faile");
      break;
  }

  KM_Delay_ms(2000);  // Delay to allow message display
}






int VerifyFingerprint(int ID)
{
	Send_Command_Check_Response(VERIFY,ID, 700000);

  error_code = GetErrorCode();       //Get the error code

  switch(error_code)
  {
		case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;       //Database is Empty
		
		case NACK_VERIFY_FAILED:
			return NACK_VERIFY_FAILED;   //No match for the given ID

    case NACK_COMM_ERR:
      return NACK_COMM_ERR;       //Communication error

    case NACK_DEV_ERR:
      return NACK_DEV_ERR;        //Device error

    case NACK_TIMEOUT:
      return NACK_TIMEOUT;        //Timeout error

    default:
			//Check for success response
      return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;  
  }
}



int DeleteAll(void)
{
	Send_Command_Check_Response(DELETEALL,0, 700000);

  error_code = GetErrorCode();        //Get the error code

  switch(error_code)
  {
		case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;       //No fingerprints in the database

    case NACK_COMM_ERR:
      return NACK_COMM_ERR;          //Communication failure

    case NACK_DEV_ERR:
      return NACK_DEV_ERR;           //Sensor hardware failure

    case NACK_TIMEOUT:
      return NACK_TIMEOUT;          //Timeout error

    default:
			//Check for success response
      return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;
    }
}



int DeleteID(int ID)
{
	Send_Command_Check_Response(DELETEALL,ID, 700000);

  error_code = GetErrorCode();        //Get the error code

  switch(error_code)
  {
		case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;       //No fingerprints in the database

    case NACK_COMM_ERR:
      return NACK_COMM_ERR;          //Communication failure

    case NACK_DEV_ERR:
      return NACK_DEV_ERR;           //Sensor hardware failure

    case NACK_TIMEOUT:
      return NACK_TIMEOUT;          //Timeout error

    default:
			//Check for success response
      return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;
	}
}








void Task1_ReadRTC_WriteLCD_1Sec(void)
{
	char buffer[100];

unsigned char cur_date[30];
				KM_I2C_Mem_Read(RTC_ADDR,0x00,0x1,cur_date,7);
				sprintf(buffer,"%02d:%02d:%02d",(cur_date[2]-6*(cur_date[2]>>4)),(cur_date[1]-6*(cur_date[1]>>4)),(cur_date[0]-6*(cur_date[0]>>4)));
				KM_LCD_Write_Cmd(0x80);
				KM_LCD_Write_Str((unsigned char *)buffer);
				
				sprintf(buffer,"%02d/%02d/%02d",cur_date[4]-6*(cur_date[4]>>4),cur_date[5]-6*(cur_date[5]>>4),cur_date[6]-6*(cur_date[6]>>4));
				KM_LCD_Write_Cmd(0xC0);
				KM_LCD_Write_Str((unsigned char *)buffer);
	
}
