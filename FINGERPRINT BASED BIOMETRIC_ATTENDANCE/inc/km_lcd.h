/*
*File Name 	:km_lcd.h
*Description :this file contains all function declarations of lcd
*/


void Lcd_gpiob_init_config(void);
void KM_LCD_Init(void);

void write_high_nibble( unsigned char data);
void write_low_nibble( unsigned char data);
void KM_LCD_Write_Cmd( unsigned char data);
void KM_LCD_Write_Data( unsigned char data);
void KM_LCD_Write_Str(unsigned char str[]);

