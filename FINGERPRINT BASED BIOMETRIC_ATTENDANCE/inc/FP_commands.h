/*
*File Name 	:FP_commands.h
*Description :This file contain all GT-521F32 fingerprint sensor commands and Macros.
*/
void Fingerprint_Response(unsigned int Timeout_Value);
void Send_Command_Check_Response(unsigned char command, unsigned short parameter,unsigned int Timeout_Value);
void PreparePacket(unsigned char packet[],unsigned char command,unsigned char parameter);
int GetErrorCode(void);
int IsFingerPlaced(void);
int CaptureFinger(void);
int EnrollStart(unsigned short int ID);
int Enroll1(void);
int Enroll2(void);
int Enroll3(void);
int GetEnrollCount(void);
int DeleteAll(void);
void EnrollFingerprint(void);

void VerifyFingerprint_LCD(int ID);
int VerifyFingerprint(int ID);

void IdentifyFingerprint_LCD(void);
int IdentifyFingerprint(void);
void Task1_ReadRTC_WriteLCD_1Sec(void);

///INPUT COMMANDS
#define OPEN                0X01                 //initialization
#define CLOSE               0x02                 //termination
#define USBINTERNAL CHECK   0X03                 //check if the connected usb device is valid
#define CHANGEBAUDRATE      0X04                 //change UART baud rste
#define SETIAPMODE          0x05                 //enter IAP mode inthis mode,FW upgrade is available
#define CMOSLED             0x12                 //control CMOS LED
#define GETENROLLCOUNT      0X20                 //get enrolled fingerprint count
#define CHECKENROLLED       0X21                 //check whether the specified id is already enrolled
#define ENROLLSTART         0X22                 //start an enrollment
#define ENROLL1             0X23                 //make 1st template for anenrollment
#define ENROLL2             0X24                 //make 2nd template for an enrollment
#define ENROLL3             0X25                 //make 3rd template for an enrollment,merge three templates into one template,save merged template to the database
#define ISPRESSFINGER       0X26                 //check if a finger is placed on the sensor
#define DELETEID            0X40                 //delete the fingerprint with the specified ID
#define DELETEALL           0X41                 //delete all fingerprints from the database
#define VERIFY              0X50                 //1:1 verification of the capturefingerprint image with the specified ID
#define IDENTIFY            0X51                 //1:N identification of the capture fingerprint image with the database
#define VERIFYTEMPLATE      0X52                 //1:1 verificatioin of a fingerprint template with the database
#define IDENTIFYTEMPLATE    0X53                 //1:N identification of a fingerprint template with the database
#define CAPTUREFINGER       0X60                 //capture a fingerprinyt image(256x256)from the sensor
#define MAKETEMPLATE        0X61                 //make template for transmission
#define GETIMAGE            0X62                 //download the captured fingerprint image(256x256)
#define GETRAWIMAGE         0X63                //capture and download raw fingerprint image(320x240)
#define GETTEMPLATE         0X70                 //Download the template of the specified id
#define SETTEMPLATE         0X71                 //upload the template of the specified id
#define GETDATABASESTART    0X72                 //start database download,obsolete
#define GETDATABASEEND      0X73                 //end database download,obsolete
#define UPGRADE_FIRMWARE    0X80                 //not supported
#define UPGRADE_ISOCD_IMAGE 0X81                 //not supported
#define SETSECURITYLEVEL    0XF0                 //set security level
#define GETSECURITYLEVEL    0XF1                 //get security level
#define SUCCESS 								 0x30  
#define ERROR_ENROLLMENT_FAIL    0x31

#define NACK_TIMEOUT                  0X1001                //obsolete,capture timeout
#define NACK_INVALID_BAUDRATE         0X1002                //obsolete,invalid serial baud rate
#define NACK_INVALID_POS              0X1003                //the specified ID is not in between 0-199
#define NACK_IS_NOT_USED              0X1004                //the specified ID is not used
#define NACK_IS_ALREADY_USED          0X1005                //the specified ID is already used
#define NACK_COMM_ERR                 0X1006                //Communication error
#define NACK_VERIFY_FAILED            0X1007                //1:1 Verification failure
#define NACK_IDENTIFY_FAILED          0X1008                //1:N Identifivation failure
#define NACK_DB_IS_FULL               0X1009                //The database is full
#define NACK_DB_IS_EMPTY              0X100A                //The database is empty
#define NACK_TURN_ERR                 0X100B                //OBSOLETE,Invalid order of the enrollment
#define NACK_BAD_FINGER               0X100C                //Too bad fingerprint
#define NACK_ENROLL_FAILED            0X100D                //Enrollment failure
#define NACK_IS_NOT_SUPPORTED         0X100E                //THE soecified command is not supported
#define NACK_DEV_ERR                  0X100F                //Device error,especially if crypto-chip is trouble
#define NACK_CAPTURE_CANCELLED        0X1010                //OBSOLETE,the capturing is cancelled
#define NACK_INVALID_PARAM            0X1011                //Invalid parameter
#define NACK_FINGER_IS_NOT_PRESSED    0X1012                //Finger is not pressed





//#define OPEN              {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x01}
//#define CLOSE             {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x02,0x01}
//#define LED_ON            {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x12,0x00,0x13,0x01}
//#define LED_OFF           {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x12,0x01}
//#define ENROLL_START      {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x22,0x00,0x23,0x01}
//#define ISPRESS_FINGER    {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x26,0x00,0x26,0x01}
//#define CAPTURE_FINGER    {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x01}
//#define ENROLL1           {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x23,0x00,0x23,0x01}
//#define ENROLL2           {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x24,0x00,0x24,0x01}
//#define ENROLL3           {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x25,0x00,0x25,0x01}
//#define CHECK_ENROLLED    {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x21,0x00,0x22,0x01}
//#define GET_ENROLL_COUNT  {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x20,0x01}
//#define IDENTIFY          {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x51,0x00,0x51,0x01}
//#define VERIFY            {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x50,0x00,0x51,0x01}
//#define DELETE_ID         {0x55,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x40,0x00,0x41,0x01}
//#define DELETE_ALL        {0x55,0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x41,0x00,0x41,0x01}
